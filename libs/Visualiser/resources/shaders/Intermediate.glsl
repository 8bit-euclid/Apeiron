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
   vec3 FragmentPosition;
   vec2 TextureCoordinate;
   mat4 ProjectionMatrix;
} data_out;

uniform mat4 u_model_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_projection_matrix;

void main()
{
   const vec4 vertex_position = vec4(position, 1.0);
   gl_Position = u_view_matrix * u_model_matrix * vertex_position;

   data_out.Normal = mat3(transpose(inverse(u_model_matrix))) * normal; // Accounts for model rotation and non-uniform scaling
   data_out.Colour = colour;
   data_out.FragmentPosition = (u_model_matrix * vertex_position).xyz;
   data_out.TextureCoordinate = texture_coordinate;
   data_out.ProjectionMatrix = u_projection_matrix;
}

#shader geometry
#version 460 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

out vec3 v_normal;
out vec3 v_colour;
out vec3 v_fragment_position;
out vec2 v_texture_coordinate;

in Data
{
   vec3 Normal;
   vec3 Colour;
   vec3 FragmentPosition;
   vec2 TextureCoordinate;
   mat4 ProjectionMatrix;
} data_in[];

void main()
{
   vec3 tangent0 = vec3(gl_in[1].gl_Position - gl_in[0].gl_Position);
   vec3 tangent1 = vec3(gl_in[2].gl_Position - gl_in[0].gl_Position);
   vec3 triangle_normal = normalize(cross(tangent0, tangent1));

   for(int i = 0; i < gl_in.length(); i++)
   {
//      gl_Position = data_in[i].ProjectionMatrix * (gl_in[i].gl_Position + 0.1*vec4(normalize(cross(tangent0, tangent1)), 0.0));
      gl_Position = data_in[i].ProjectionMatrix * gl_in[i].gl_Position;
      v_normal = triangle_normal;
//      v_normal = data_in[i].Normal;
      v_colour = data_in[i].Colour;
      v_fragment_position = data_in[i].FragmentPosition;
      v_texture_coordinate = data_in[i].TextureCoordinate;
      EmitVertex();
   }
   EndPrimitive();
}

#shader fragment
#version 460 core
#extension GL_OES_standard_derivatives : enable

in vec3 v_normal;
in vec3 v_colour;
in vec3 v_fragment_position;
in vec2 v_texture_coordinate;

out vec4 colour;

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

vec4 CalculateLightByDirection(Light _light, vec3 _direction)
{
//   vec3 face_normal = normalize(cross(dFdx(v_fragment_position), dFdy(v_fragment_position)));
//   if(!gl_FrontFacing) face_normal = -face_normal;

   vec4 ambient_colour = _light.AmbientIntensity * _light.Colour;

//   float diffuse_factor = max(dot(normalize(face_normal), normalize(_direction)), 0.0);
   float diffuse_factor = max(dot(normalize(v_normal), normalize(_direction)), 0.0);
   vec4 diffuse_colour = diffuse_factor * _light.DiffuseIntensity * _light.Colour;

   vec4 specular_colour = vec4(0.0);
   if(diffuse_factor > 0.0)
   {
      vec3 fragment_to_camera = normalize(u_camera_position - v_fragment_position);
//      vec3 reflected_ray = normalize(reflect(_direction, normalize(face_normal)));
      vec3 reflected_ray = normalize(reflect(_direction, normalize(v_normal)));

      float specular_factor = dot(fragment_to_camera, reflected_ray);
      if(specular_factor > 0.0)
      {
         specular_factor = pow(specular_factor, u_material.Smoothness);
         specular_colour = specular_factor * u_material.SpecularIntensity * _light.Colour;
      }

   }

   return (ambient_colour + diffuse_colour + specular_colour);
}

vec4 CalculateDirectionalLight()
{
   return CalculateLightByDirection(u_directional_light.Base, u_directional_light.Direction);
}

vec4 CalculatePointLight(PointLight _point_light)
{
   vec3 direction = v_fragment_position - _point_light.Position;
   float distance = length(direction);
   direction = normalize(direction);

   const float attenuation = _point_light.AttenuationCoefficients[0] +
   _point_light.AttenuationCoefficients[1] * distance +
   _point_light.AttenuationCoefficients[2] * distance * distance;

   return CalculateLightByDirection(_point_light.Base, direction) / attenuation;
}

vec4 CalculatePointLights()
{
   vec4 total_colour = vec4(0.0, 0.0, 0.0, 0.0);
   for(int i = 0; i < u_point_light_count; i++) total_colour += CalculatePointLight(u_point_lights[i]);
   return total_colour;
}

vec4 CalculateSpotLight(SpotLight _spot_light)
{
   vec3 ray_direction = normalize(v_fragment_position - _spot_light.Point.Position);
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

//   vec4 texture_colour = texture(u_texture, v_texture_coordinate);
//   colour = texture_colour * lighting;

   colour = u_colour * lighting;

//   colour = v_position * lighting;
}