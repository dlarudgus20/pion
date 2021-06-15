#pragma once

class shader;

struct light
{
    glm::vec3 pos;
    glm::vec4 ambient;
    glm::vec4 diffuse;
    glm::vec4 specular;

    void apply(shader& s) const;
};
