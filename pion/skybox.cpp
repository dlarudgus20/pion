#include "pch.h"
#include "skybox.h"
#include "scene.h"
#include "camera.h"

#include "tdogl/Bitmap.h"

namespace
{
    const GLfloat vertices[] = {
        -1.0f, 1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,

        -1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f
    };
    constexpr GLsizei strides = 3 * sizeof(GLfloat);
}

skybox::skybox()
    : m_shader("shaders/skybox.vert", "shaders/skybox.frag")
{
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);

    glBindVertexArray(m_vao);
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, strides, (GLvoid*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &m_cube_map);

    glBindTexture(GL_TEXTURE_CUBE_MAP, m_cube_map);
    {
        auto bmp1 = tdogl::Bitmap::bitmapFromFile("textures/cubemap1.jpg");
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 0, 0, GL_RGB, bmp1.width(), bmp1.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, bmp1.pixelBuffer());

        auto bmp2 = tdogl::Bitmap::bitmapFromFile("textures/cubemap2.jpg");
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 1, 0, GL_RGB, bmp2.width(), bmp2.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, bmp2.pixelBuffer());

        auto bmp3 = tdogl::Bitmap::bitmapFromFile("textures/cubemap3.jpg");
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 2, 0, GL_RGB, bmp3.width(), bmp3.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, bmp3.pixelBuffer());

        auto bmp4 = tdogl::Bitmap::bitmapFromFile("textures/cubemap4.jpg");
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 3, 0, GL_RGB, bmp4.width(), bmp4.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, bmp4.pixelBuffer());

        auto bmp5 = tdogl::Bitmap::bitmapFromFile("textures/cubemap5.jpg");
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 4, 0, GL_RGB, bmp5.width(), bmp5.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, bmp5.pixelBuffer());

        auto bmp6 = tdogl::Bitmap::bitmapFromFile("textures/cubemap6.jpg");
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 5, 0, GL_RGB, bmp6.width(), bmp6.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, bmp6.pixelBuffer());

        glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

skybox::~skybox()
{
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
}

void skybox::draw(scene& sc)
{
    auto& projection = sc.get_camera().get_projection();
    auto& view = sc.get_camera().get_view();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(30));

    m_shader.use();
    m_shader.setUniform4fm("projection", projection);
    m_shader.setUniform4fm("view_model", view * model);

    glDepthMask(GL_FALSE);
    glEnable(GL_CULL_FACE);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_cube_map);

    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / strides);
    glBindVertexArray(0);

    glDepthMask(GL_TRUE);
}
