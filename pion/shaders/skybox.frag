#version 330 core

uniform samplerCube skybox;

in VS_OUT
{
    vec3 tex_coord;
} vs_out;

out vec4 color;

void main()
{
    color = texture(skybox, vs_out.tex_coord);
}
