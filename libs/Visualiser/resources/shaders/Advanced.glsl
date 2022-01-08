#shader vertex
#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 colour;
layout(location = 3) in vec2 texture_coordinate;

out Data
{
   vec3 Normal;
   vec3 Colour;
   vec2 TextureCoordinate;
   vec3 FragmentPosition;
   vec4 FragmentPositionDlight;

   mat4 ViewProjMatrix;
} v_data_out;

uniform mat4 u_model_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_projection_matrix;
uniform mat4 u_dlight_space_matrix;

void main()
{
   gl_Position = u_model_matrix * vec4(position, 1.0);

   v_data_out.Normal = mat3(transpose(inverse(u_model_matrix))) * normal; // Accounts for model rotation and non-uniform scaling
   v_data_out.Colour = colour;
   v_data_out.TextureCoordinate = texture_coordinate;
   v_data_out.FragmentPosition = gl_Position.xyz;
   v_data_out.FragmentPositionDlight = u_dlight_space_matrix * vec4(v_data_out.FragmentPosition, 1.0);

   v_data_out.ViewProjMatrix = u_projection_matrix * u_view_matrix;
}




#shader geometry
#version 460 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in Data
{
   vec3 Normal;
   vec3 Colour;
   vec2 TextureCoordinate;
   vec3 FragmentPosition;
   vec4 FragmentPositionDlight;

   mat4 ViewProjMatrix;
} v_data_in[];

out Data
{
   vec3 Normal;
   vec3 Colour;
   vec2 TextureCoordinate;
   vec3 FragmentPosition;
   vec4 FragmentPositionDlight;
} v_data_out;

void main()
{
   const vec3 tangent0 = (gl_in[1].gl_Position - gl_in[0].gl_Position).xyz;
   const vec3 tangent1 = (gl_in[2].gl_Position - gl_in[0].gl_Position).xyz;
   const vec3 face_normal = normalize(cross(tangent0, tangent1));

   for(int i = 0; i < gl_in.length(); i++)
   {
//      gl_Position = v_data_in[i].ViewProjMatrix * (gl_in[i].gl_Position + 0.5*vec4(face_normal, 0.0));
      gl_Position = v_data_in[i].ViewProjMatrix * gl_in[i].gl_Position;

      v_data_out.Normal = face_normal; // Flat shading
//      v_data_out.Normal = v_data_in[i].Normal; // Smooth shading

      v_data_out.Colour = v_data_in[i].Colour;
      v_data_out.TextureCoordinate = v_data_in[i].TextureCoordinate;
      v_data_out.FragmentPosition = v_data_in[i].FragmentPosition;
      v_data_out.FragmentPositionDlight = v_data_in[i].FragmentPositionDlight;

      EmitVertex();
   }

   EndPrimitive();
}




#shader fragment
#version 460 core

in Data
{
   vec3 Normal;
   vec3 Colour;
   vec2 TextureCoordinate;
   vec3 FragmentPosition;
   vec4 FragmentPositionDlight;
} v_data_in;

out vec4 fragment_colour;

struct Light
{
   vec4 Colour;
   float AmbientIntensity;
   float DiffuseIntensity;
};

struct DirectionalLight
{
   Light Base;
   vec3 Direction;
};

struct PointLight
{
   Light Base;
   vec3 Position;
   vec3 AttenuationCoefficients;
   samplerCube Shadow;
};

struct SpotLight
{
   PointLight Point;
   vec3 Direction;
   float CosConeAngle;
};

struct Material
{
   float SpecularIntensity;
   float Smoothness;
};

uniform vec4 u_colour;
uniform vec3 u_camera_position;
uniform sampler2D u_texture;

uniform Material u_material;

const int Max_Point_Lights = 4;
const int Max_Spot_Lights = 4;
uniform int u_point_light_count;
uniform int u_spot_light_count;
uniform DirectionalLight u_directional_light;
uniform PointLight u_point_lights[Max_Point_Lights];
uniform SpotLight u_spot_lights[Max_Spot_Lights];
uniform sampler2D u_direc_shadow;
uniform float u_plight_far_plane;

const vec3 sampleOffsetDirections[20] = { vec3( 1, 1, 1), vec3( 1, -1, 1), vec3(-1, -1, 1), vec3(-1, 1, 1),
                                          vec3( 1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
                                          vec3( 1, 1, 0), vec3( 1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
                                          vec3( 1, 0, 1), vec3(-1, 0, 1), vec3(1, 0, -1), vec3(-1, 0, -1),
                                          vec3( 0, 1, 1), vec3( 0, -1, 1), vec3(0, -1, -1), vec3(0, 1, -1) };

float CalculateDirectionalLightShadow()
{
   vec3 projected_coordinates = v_data_in.FragmentPositionDlight.xyz / v_data_in.FragmentPositionDlight.w;
   projected_coordinates = (projected_coordinates + 1.0f) / 2.0f;

   float actual_depth = projected_coordinates.z;
   float shadow = 0.0f;
   if(actual_depth <= 1.0f)
   {
      vec2 texel_size = 1.0f / textureSize(u_direc_shadow, 0);
      const float bias = max(0.05 * (1.0 - dot(normalize(v_data_in.Normal), normalize(u_directional_light.Direction))), 0.005);

      for(int x = -1; x <= 1; x++)
         for(int y = -1; y <= 1; y++)
         {
            float pcf_depth = bias + texture(u_direc_shadow, projected_coordinates.xy + vec2(x, y) * texel_size).r;
            shadow += actual_depth > pcf_depth ? 1.0f : 0.0f;
         }
      shadow /= 9.0f;
   }

   return shadow;
}

float CalculatePointLightShadow(PointLight _point_light)
{

   const vec3 light_to_fragment = v_data_in.FragmentPosition - _point_light.Position;
   const float current_depth = length(light_to_fragment);

   const float bias = 0.15;
   const int n_samples = 20;
   const float view_distance = length(v_data_in.FragmentPosition - u_camera_position);
   const float disk_radius = (1.0 + (view_distance / u_plight_far_plane)) / 25.0;;
   float shadow = 0.0;
   for(int i = 0; i < n_samples; ++i)
   {
      const float closest_depth = bias + u_plight_far_plane * texture(_point_light.Shadow, light_to_fragment + disk_radius * sampleOffsetDirections[i]).r;
      if(current_depth > closest_depth) shadow += 1.0f;
   }
   shadow /= float(n_samples);

   return shadow;
}

vec4 CalculateLightByDirection(Light _light, vec3 _direction, float _shadow_factor)
{
   const vec4 ambient_colour = _light.AmbientIntensity * _light.Colour;

   const float diffuse_factor = max(dot(-v_data_in.Normal, normalize(_direction)), 0.0f);
   const vec4 diffuse_colour = diffuse_factor * _light.DiffuseIntensity * _light.Colour;

   const vec3 fragment_to_camera = normalize(u_camera_position - v_data_in.FragmentPosition);
   const vec3 fragment_to_light = normalize(-_direction);
   const vec3 halfway_direction = normalize(fragment_to_light + fragment_to_camera);
   const float specular_factor = pow(max(dot(halfway_direction, v_data_in.Normal), 0.0f), u_material.Smoothness);
   const vec4 specular_colour = specular_factor * u_material.SpecularIntensity * _light.Colour;

   return ambient_colour + (1.0f - _shadow_factor) * (diffuse_colour + specular_colour);
}

vec4 CalculateDirectionalLight()
{
   return CalculateLightByDirection(u_directional_light.Base, u_directional_light.Direction, CalculateDirectionalLightShadow());
}

vec4 CalculatePointLight(PointLight _point_light)
{
   vec3 light_to_fragment = v_data_in.FragmentPosition - _point_light.Position;
   float distance = length(light_to_fragment);
   light_to_fragment = normalize(light_to_fragment);

   const float attenuation = _point_light.AttenuationCoefficients[0] +
                             _point_light.AttenuationCoefficients[1] * distance +
                             _point_light.AttenuationCoefficients[2] * distance * distance;

   return CalculateLightByDirection(_point_light.Base, light_to_fragment, CalculatePointLightShadow(_point_light)) / attenuation;
}

vec4 CalculatePointLights()
{
   vec4 total_colour = vec4(0.0, 0.0, 0.0, 0.0);
   for(int i = 0; i < u_point_light_count; ++i) total_colour += CalculatePointLight(u_point_lights[i]);
   return total_colour;
}

vec4 CalculateSpotLight(SpotLight _spot_light)
{
   vec3 ray_direction = normalize(v_data_in.FragmentPosition - _spot_light.Point.Position);
   float spot_light_factor = dot(ray_direction, _spot_light.Direction);

   if(spot_light_factor > _spot_light.CosConeAngle)
      return (1.0f - (1.0f - spot_light_factor ) / (1.0f - _spot_light.CosConeAngle)) * CalculatePointLight(_spot_light.Point);
   else return vec4(0.0, 0.0, 0.0, 0.0);
}

vec4 CalculateSpotLights()
{
   vec4 total_colour = vec4(0.0, 0.0, 0.0, 0.0);
   for(int i = 0; i < u_spot_light_count; i++) total_colour += CalculateSpotLight(u_spot_lights[i]);
   return total_colour;
}

void main()
{
   vec4 lighting = CalculateDirectionalLight() + CalculatePointLights() + CalculateSpotLights();

   fragment_colour = lighting;
//   fragment_colour = vec4(vec3(CalculatePointLightShadow(u_point_lights[0]) / u_plight_far_plane), 1.0f);
//   fragment_colour = vec4(lighting * texture_colour, 1.0f);
//   fragment_colour = u_colour * lighting;
}