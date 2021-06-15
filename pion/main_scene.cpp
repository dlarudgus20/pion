#include "pch.h"
#include "main_scene.h"
#include "main_window.h"

main_scene::main_scene(main_window& wnd)
    : m_wnd(wnd)
{
    m_light.pos = glm::vec3(0, -200, 0);
    m_light.ambient = glm::vec4(1.0f);
    m_light.diffuse = glm::vec4(1.0f);
    m_light.specular = glm::vec4(1.0f);
}

void main_scene::update(float dt)
{
    if (glfwGetKey(m_wnd, GLFW_KEY_Z) != GLFW_PRESS)
    {
        m_ps.add_particle_bunch(dt);
        m_ps.update(dt);

        constexpr float speed = 8.0f;
        m_camera.rotate_around(glm::radians(speed) * dt, glm::vec3(0.0f, 1.0f, 0.0f));
    }

    float dx = 0, dy = 0, dz = 0;
    if (glfwGetKey(m_wnd, GLFW_KEY_A) == GLFW_PRESS)
        dx -= 1;
    if (glfwGetKey(m_wnd, GLFW_KEY_D) == GLFW_PRESS)
        dx += 1;
    if (glfwGetKey(m_wnd, GLFW_KEY_S) == GLFW_PRESS)
        dy -= 1;
    if (glfwGetKey(m_wnd, GLFW_KEY_W) == GLFW_PRESS)
        dy += 1;
    if (glfwGetKey(m_wnd, GLFW_KEY_R) == GLFW_PRESS)
        dz -= 1;
    if (glfwGetKey(m_wnd, GLFW_KEY_F) == GLFW_PRESS)
        dz += 1;

    m_camera.move(10.0f * dt * dx, 10.0f * dt * dy, 10.0f * dt * dz);
    m_camera.update_view();

    m_elapsedTime += dt;
    if (m_tick != int(m_elapsedTime))
    {
        std::string title = "pion [particles: " + std::to_string(m_ps.particle_count()) + ", FPS: " + std::to_string(m_wnd.fps()) + "]";
        glfwSetWindowTitle(m_wnd, title.c_str());
        m_tick = int(m_elapsedTime);
    }
}

void main_scene::draw()
{
    m_skybox.draw(*this);
    m_ps.draw(*this);
}

camera& main_scene::get_camera()
{
    return m_camera;
}

skybox& main_scene::get_skybox()
{
    return m_skybox;
}

light& main_scene::get_light()
{
    return m_light;
}
