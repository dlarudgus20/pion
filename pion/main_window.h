#pragma once

#include "scene.h"

class main_window
{
public:
    main_window(const main_window&) = delete;
    main_window& operator =(const main_window&) = delete;

    main_window();

    void initialize();
    void loop();

    float fps() const;

    operator GLFWwindow*() const
    {
        return m_wnd;
    }

    const glm::vec2& get_mouse() const
    {
        return m_mouse;
    }
    const glm::vec2& get_prev_mouse() const
    {
        return m_prev_mouse;
    }

private:
    void on_display();
    void on_idle();
    void on_cursor_pos(double xpos, double ypos);

    GLFWwindow* m_wnd;
    long long m_prev_time;

    long long m_prev_frame;
    long long m_now_frame;

    glm::vec2 m_mouse;
    glm::vec2 m_prev_mouse;

    std::unique_ptr<scene> m_ptr_scene;
};
