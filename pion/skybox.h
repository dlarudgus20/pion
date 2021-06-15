#pragma once

#include "shader.h"

class scene;

class skybox
{
public:
    skybox(const skybox&) = delete;
    skybox& operator =(const skybox&) = delete;

    skybox();
    ~skybox();

    void draw(scene& sc);

    GLuint get_cube_map() const
    {
        return m_cube_map;
    }

private:
    GLuint m_vbo, m_vao;
    GLuint m_cube_map;

    shader m_shader;
};