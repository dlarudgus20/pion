#include "pch.h"
#include "light.h"
#include "shader.h"

void light::apply(shader& s) const
{
    s.setUniform3f("light.pos", pos);
    s.setUniform4f("light.ambient", ambient);
    s.setUniform4f("light.diffuse", diffuse);
    s.setUniform4f("light.specular", specular);
}
