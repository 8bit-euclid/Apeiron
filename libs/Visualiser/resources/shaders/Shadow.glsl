#shader vertex
#version 460 core

layout(location = 0) in vec3 position;

uniform mat4 u_model_matrix;
uniform mat4 u_direc_light_matrix; // projection * view (from the directional light's perspective)

void main()
{
    gl_Position = u_direc_light_matrix * u_model_matrix * vec4(position, 1.0);
}



#shader fragment
#version 460 core

void main()
{

}