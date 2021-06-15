#pragma once

class shader;

struct material
{
    glm::vec4 ambient;
    glm::vec4 diffuse;
    glm::vec4 specular;
    float shininess;
    float refr_index;
    float refl_cof;
    float refr_cof;

    void apply(shader& s) const;
};
