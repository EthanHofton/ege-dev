#version 410 core

layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 u_projection;
uniform mat4 u_view;

void main()
{
    TexCoords = aPos;
    mat4 view = mat4(mat3(u_view)); // remove translation from the view matrix
    gl_Position = u_projection * view * vec4(aPos, 1.0);
}  
