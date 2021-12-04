#shader vertex
#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 colour;
layout(location = 3) in vec2 texture_coordinate;

out vec4 v_position;
out vec3 v_normal;
out vec3 v_colour;
out vec3 v_fragment_position;
out vec2 v_texture_coordinate;

uniform mat4 u_model_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_projection_matrix;

void main()
{
   v_position = vec4(position, 1.0);
   gl_Position = u_projection_matrix * u_view_matrix * u_model_matrix * v_position;

   v_normal = mat3(transpose(inverse(u_model_matrix))) * normal; // Accounts for model rotation and scaling
   v_colour = colour;
   v_fragment_position = (u_model_matrix * v_position).xyz;
   v_texture_coordinate = texture_coordinate;
}

#shader fragment
#version 460 core

in vec4 v_position;
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
uniform int u_point_light_count;
uniform DirectionalLight u_directional_light;
uniform PointLight u_point_lights[Max_Point_Lights];

vec4 CalculateLightByDirection(Light _light, vec3 _direction)
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

   return (ambient_colour + diffuse_colour + specular_colour);
}

vec4 CalculateDirectionalLight()
{
   return CalculateLightByDirection(u_directional_light.Base, u_directional_light.Direction);
}

vec4 CalculatePointLights()
{
   vec4 total_colour = vec4(0.0, 0.0, 0.0, 0.0);
   for(int i = 0; i < u_point_light_count; i++)
   {
      vec3 direction = v_fragment_position - u_point_lights[i].Position;
      float distance = length(direction);
      direction = normalize(direction);

      const float attenuation = u_point_lights[i].AttenuationCoefficients[0] +
                                u_point_lights[i].AttenuationCoefficients[1] * distance +
                                u_point_lights[i].AttenuationCoefficients[2] * distance * distance;

      total_colour += CalculateLightByDirection(u_point_lights[i].Base, direction) / attenuation;
   }

   return total_colour;
}

void main()
{
   vec4 lighting = CalculateDirectionalLight() + CalculatePointLights();

//   vec4 texture_colour = texture(u_texture, v_texture_coordinate);
//   colour = texture_colour * lighting;

   colour = u_colour * lighting;

//   colour = v_position * lighting;
}