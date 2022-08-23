#shader vertex
#version 460 core

layout(location = 0) in vec3 position;
layout(location = 4) in vec2 texture_coordinate;

out vec2 v_texture_coordinate;

void main()
{
    gl_Position = vec4(position.x, position.y, 0.0, 1.0);
    v_texture_coordinate = texture_coordinate;
}



#shader fragment
#version 460 core

in vec2 v_texture_coordinate;

out vec4 fragment_colour;

uniform sampler2D screen_texture;

void main()
{
    fragment_colour = texture(screen_texture, v_texture_coordinate);
}