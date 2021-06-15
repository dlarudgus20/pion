#include "pch.h"
#include "main_window.h"
#include "main_scene.h"

namespace
{
    constexpr int WIDTH = 1600;
    constexpr int HEIGHT = 900;
    constexpr auto TITLE = "pion";

    glm::vec2 mouse_coord(double xpos, double ypos)
    {
        return glm::vec2(float(xpos * 2 / WIDTH - 1), -float(ypos * 2 / HEIGHT - 1));
    }
}

main_window::main_window()
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    m_wnd = glfwCreateWindow(WIDTH, HEIGHT, TITLE, nullptr, nullptr);

    glfwMakeContextCurrent(m_wnd);
    glfwSetWindowUserPointer(m_wnd, this);

#define THIZ static_cast<main_window*>(glfwGetWindowUserPointer(wnd))
    glfwSetCursorPosCallback(m_wnd, [](GLFWwindow* wnd, double xpos, double ypos) {
        THIZ->on_cursor_pos(xpos, ypos);
        });
    glfwSetWindowCloseCallback(m_wnd, [](GLFWwindow* wnd) {
        glfwSetWindowShouldClose(wnd, GL_TRUE);
        });
#undef THIZ
}

void main_window::initialize()
{
    m_prev_time = std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::steady_clock::now().time_since_epoch()).count();

    m_prev_frame = m_now_frame = m_prev_time;

    double xpos, ypos;
    glfwGetCursorPos(m_wnd, &xpos, &ypos);
    m_prev_mouse = m_mouse = mouse_coord(xpos, ypos);

    m_ptr_scene = std::make_unique<main_scene>(*this);
}

void main_window::loop()
{
    while (!glfwWindowShouldClose(m_wnd))
    {
        long long now = std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::steady_clock::now().time_since_epoch()).count();

        glfwPollEvents();
        on_display();
        on_idle();

        m_prev_frame = m_now_frame;
        m_now_frame = now;
    }
}

float main_window::fps() const
{
    long long dt = m_now_frame - m_prev_frame;
    if (dt == 0)
        return 0.0f;

    return 1000000.0f / dt;
}

void main_window::on_display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0, 0, 0, 0);

    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);

    m_ptr_scene->draw();

    glfwSwapBuffers(m_wnd);
}

void main_window::on_idle()
{
    long long nowTime = std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::steady_clock::now().time_since_epoch()).count();

    float dt = (nowTime - m_prev_time) / 1000000.0f;

    m_ptr_scene->update(dt);

    m_prev_time = nowTime;
    m_prev_mouse = m_mouse;
}

void main_window::on_cursor_pos(double xpos, double ypos)
{
    m_mouse = mouse_coord(xpos, ypos);
}
