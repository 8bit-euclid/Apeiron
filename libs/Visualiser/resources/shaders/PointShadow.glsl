#shader vertex
#version 460 core

layout(location = 0) in vec3 position;

uniform mat4 u_model_matrix;

void main()
{
    gl_Position = u_model_matrix * vec4(position, 1.0);
}




#shader geometry
#version 460 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 18) out;

out vec4 v_fragment_position;

uniform mat4 u_plight_space_matrices[6];

void main()
{
    for(int face = 0; face < 6; ++face)
    {
        gl_Layer = face; // Index of the cube map face to render to
        for(int i = 0; i < gl_in.length(); ++i)
        {
            v_fragment_position = gl_in[i].gl_Position;
            gl_Position = u_plight_space_matrices[face] * v_fragment_position;

            EmitVertex();
        }
        EndPrimitive();
    }
}




#shader fragment
#version 460 core

in vec4 v_fragment_position;

uniform float u_plight_far_plane;
uniform vec3 u_plight_position;

void main()
{
    gl_FragDepth = length(v_fragment_position.xyz - u_plight_position) / u_plight_far_plane; // Division maps depth to [0,1]
}