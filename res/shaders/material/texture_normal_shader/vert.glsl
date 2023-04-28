#version 410 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texcoords;

out vec3 v_position;
out vec3 v_normal;
out vec2 v_tex_coords;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
    v_position = vec3(u_model * vec4(position, 1.0));
    v_normal = mat3(transpose(inverse(u_model))) * normal;
    v_tex_coords = texcoords;

    gl_Position = u_projection * u_view * vec4(v_position, 1.0);
}
