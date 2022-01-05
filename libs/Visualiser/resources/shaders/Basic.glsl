#shader vertex
#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 colour;
layout(location = 3) in vec2 texture_coordinate;

out vec3 v_normal;
out vec3 v_colour;
out vec2 v_texture_coordinate;

uniform mat4 u_model_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_projection_matrix;

void main()
{
   gl_Position = u_projection_matrix * u_view_matrix * u_model_matrix * vec4(position, 1.0);

   v_normal = mat3(transpose(inverse(u_model_matrix))) * normal; // Accounts for model rotation and non-uniform scaling
   v_colour = colour;
   v_texture_coordinate = texture_coordinate;
}

#shader fragment
#version 460 core

in vec3 v_normal;
in vec3 v_colour;
in vec2 v_texture_coordinate;

out vec4 colour;

uniform vec4 u_colour;
uniform sampler2D u_texture;

void main()
{
   vec4 texture_colour = texture(u_texture, v_texture_coordinate);
//   colour = texture_colour;
//   colour = texture_colour * u_colour;

   colour = u_colour;
}