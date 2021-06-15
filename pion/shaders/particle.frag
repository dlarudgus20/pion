#version 330 core

struct Light
{
    vec3 pos;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

struct Material
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
    float refr_index;
    float refl_cof;
    float refr_cof;
};

uniform mat3 matN;
uniform vec3 camera_pos;
uniform samplerCube skybox;

uniform Light light;
uniform Material material;

in VS_OUT
{
    vec3 pos;
    vec2 tex_coord;
} vs_out;

out vec4 color;

void main()
{
    vec2 Nxy = vs_out.tex_coord.xy * 2.0 - vec2(1.0);
    float len2Nxy = dot(Nxy, Nxy);
    if (len2Nxy > 1)
        discard;

    vec3 N = normalize(matN * vec3(Nxy, sqrt(1 - len2Nxy)));

    // phong lighting
    vec3 E = normalize(camera_pos - vs_out.pos);

    vec3 L = normalize(light.pos - vs_out.pos);
    vec3 R = normalize(-reflect(L, N));

    vec4 ambient = light.ambient * material.ambient;
    vec4 diffuse = light.diffuse * material.diffuse * max(dot(N, L), 0.0);
    diffuse = clamp(diffuse, 0.0, 1.0);

    vec4 specular = light.specular * material.specular * pow(max(dot(R, E), 0.0), 0.3 * material.shininess);
    specular = clamp(specular, 0.0, 1.0);

    // skybox reflection & refraction
    vec3 reflectRay = reflect(-E, N);
    vec3 refractRay = refract(-E, N, material.refr_index);

    vec4 refl_color = texture(skybox, reflectRay);
    vec4 refr_color = texture(skybox, refractRay);
    vec4 refl_refr = material.refl_cof * refl_color + material.refr_cof * refr_color;

    color = clamp(ambient + diffuse, 0.0, 1.0) * refl_refr + specular;
}
