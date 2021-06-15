#include "pch.h"
#include "material.h"
#include "shader.h"

void material::apply(shader& s) const
{
    s.setUniform4f("material.ambient", ambient);
    s.setUniform4f("material.diffuse", diffuse);
    s.setUniform4f("material.specular", specular);
    s.setUniform1f("material.shininess", shininess);
    s.setUniform1f("material.refr_index", refr_index);
    s.setUniform1f("material.refl_cof", refl_cof);
    s.setUniform1f("material.refr_cof", refr_cof);
}
