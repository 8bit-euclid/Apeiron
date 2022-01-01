#shader vertex
#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 colour;
layout(location = 3) in vec2 texture_coordinate;

out vec4 v_position;
out vec3 v_normal;
out vec3 v_colour;
out vec2 v_texture_coordinate;
out vec3 v_fragment_position;
out vec4 v_fragment_position_dlight;

uniform mat4 u_model_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_projection_matrix;
uniform mat4 u_direc_light_matrix; // projection * view (from the directional light's perspective)

void main()
{
   v_position = vec4(position, 1.0);
   gl_Position = u_projection_matrix * u_view_matrix * u_model_matrix * v_position;

   v_normal = mat3(transpose(inverse(u_model_matrix))) * normal; // Accounts for model rotation and non-uniform scaling
   v_colour = colour;
   v_texture_coordinate = texture_coordinate;
   v_fragment_position = (u_model_matrix * v_position).xyz;
   v_fragment_position_dlight = u_direc_light_matrix * u_model_matrix * v_position;
}

   #shader fragment
   #version 460 core
   #extension GL_OES_standard_derivatives : enable

in vec4 v_position;
in vec3 v_normal;
in vec3 v_colour;
in vec2 v_texture_coordinate;
in vec3 v_fragment_position;
in vec4 v_fragment_position_dlight;

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
uniform sampler2D u_direc_shadow;

uniform Material u_material;

const int Max_Point_Lights = 4;
const int Max_Spot_Lights = 4;
uniform int u_point_light_count;
uniform int u_spot_light_count;
uniform DirectionalLight u_directional_light;
uniform PointLight u_point_lights[Max_Point_Lights];
uniform SpotLight u_spot_lights[Max_Spot_Lights];

float CalculateDirectionalShadow()
{
   vec3 projected_coordinates = v_fragment_position_dlight.xyz / v_fragment_position_dlight.w;
   projected_coordinates = (projected_coordinates * 0.5) + 0.5;

   float actualDepth = projected_coordinates.z;
   float shadow = 0.0f;
   if(actualDepth <= 1.0f)
   {
      vec2 texel_size = 1.0f / textureSize(u_direc_shadow, 0);
      float bias = max(0.05 * (1.0 - dot(normalize(v_normal), normalize(u_directional_light.Direction))), 0.005);

      for(int x = -1; x <= 1; x++)
      for(int y = -1; y <= 1; y++)
      {
         float pcf_depth = bias + texture(u_direc_shadow, projected_coordinates.xy + vec2(x, y) * texel_size).r;
         shadow += actualDepth > pcf_depth ? 1.0f : 0.0f;
      }
      shadow /= 9.0f;
   }

   return shadow;
}

vec4 CalculateLightByDirection(Light _light, vec3 _direction, float _shadow_factor)
{
   vec4 ambient_colour = _light.AmbientIntensity * _light.Colour;

   float diffuse_factor = max(dot(normalize(v_normal), normalize(_direction)), 0.0);
   vec4 diffuse_colour = diffuse_factor * _light.DiffuseIntensity * _light.Colour;

   vec4 specular_colour = vec4(0.0);
   if(diffuse_factor > 0.0)
   {
      vec3 fragment_to_camera = normalize(u_camera_position - v_fragment_position);
      vec3 reflected_ray = normalize(reflect(_direction, normalize(v_normal)));

      float specular_factor = dot(fragment_to_camera, reflected_ray);
      if(specular_factor > 0.0)
      {
         specular_factor = pow(specular_factor, u_material.Smoothness);
         specular_colour = specular_factor * u_material.SpecularIntensity * _light.Colour;
      }

   }

   return (ambient_colour + (1.0f - _shadow_factor) * (diffuse_colour + specular_colour));
}

vec4 CalculateDirectionalLight()
{
   return CalculateLightByDirection(u_directional_light.Base, u_directional_light.Direction, 0.0f);
   //   return CalculateLightByDirection(u_directional_light.Base, u_directional_light.Direction, CalculateDirectionalShadow());
}

vec4 CalculatePointLight(PointLight _point_light)
{
   vec3 direction = v_fragment_position - _point_light.Position;
   float distance = length(direction);
   direction = normalize(direction);

   const float attenuation = _point_light.AttenuationCoefficients[0] +
   _point_light.AttenuationCoefficients[1] * distance +
   _point_light.AttenuationCoefficients[2] * distance * distance;

   return CalculateLightByDirection(_point_light.Base, direction, 0.0f) / attenuation;
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

   vec4 texture_colour = texture(u_texture, v_texture_coordinate);
   //   colour = texture_colour * lighting;
   colour = texture_colour;

   //   colour = u_colour * lighting;

   //   colour = v_position * lighting;
}