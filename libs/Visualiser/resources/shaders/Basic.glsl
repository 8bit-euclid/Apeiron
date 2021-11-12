#shader vertex
#version 460 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texture_coordinate;

out vec2 v_texture_coordinate;

uniform mat4 u_mvp_matrix;

void main()
{
//   gl_Position = u_mvp_matrix * position;
   gl_Position = position;
   v_texture_coordinate = texture_coordinate;
}

#shader fragment
#version 460 core

layout(location = 0) out vec4 color;

in vec2 v_texture_coordinate;

uniform vec4 u_color;
uniform sampler2D u_texture;

void main()
{
//   vec4 texture_color = texture(u_texture, v_texture_coordinate);
//   color = texture_color;
   color = u_color;
}