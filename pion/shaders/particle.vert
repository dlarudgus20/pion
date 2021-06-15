#version 330 core

uniform mat4 view_model;
uniform mat4 projection;
uniform mat4 view_inv;

layout(location = 0) in vec2 position;

out VS_OUT
{
    vec3 pos;
    vec2 tex_coord;
} vs_out;

void main()
{
    vs_out.tex_coord = (position + vec2(1.0)) / 2.0;

    vec4 center = view_model * vec4(0.0, 0.0, 0.0, 1.0);
    vec4 view = view_model * vec4(position, 0.0, 1.0);
    view = center + length(view - center) * vec4(position, 0.0, 0.0);

    vec4 world = view_inv * view;
    vs_out.pos = world.xyz / world.w;

    gl_Position = projection * view;
}
