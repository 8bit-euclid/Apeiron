#shader vertex
#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 temp1;
layout(location = 2) in vec3 temp2;
layout(location = 3) in vec3 temp3;
layout(location = 4) in vec2 texture_coordinate;

out vec2 v_texture_coordinate;

uniform mat4 u_model_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_projection_matrix;

void main()
{
   gl_Position = u_projection_matrix * u_view_matrix * u_model_matrix * vec4(position, 1.0);
   v_texture_coordinate = texture_coordinate;
}

#shader fragment
#version 460 core

in vec2 v_texture_coordinate;

out vec4 colour;

uniform vec4 u_colour;
uniform sampler2D u_diffuse_map;

void main()
{
   colour = texture(u_diffuse_map, v_texture_coordinate);
//   colour = u_colour;
}