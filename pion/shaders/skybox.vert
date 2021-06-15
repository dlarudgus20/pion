#version 330 core

uniform mat4 view_model;
uniform mat4 projection;

layout(location = 0) in vec3 position;

out VS_OUT
{
    vec3 tex_coord;
} vs_out;

void main()
{
    vs_out.tex_coord = position;

    vec4 pos = projection * view_model * vec4(position, 0.0);
    gl_Position = vec4(pos.x, pos.y, -pos.w, pos.w);
}
