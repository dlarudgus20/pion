#pragma once

class camera
{
public:
    camera(const camera&) = delete;
    camera& operator =(const camera&) = delete;

    camera();

    void move(float dx, float dy, float dz);
    void rotate_self(float pitch, float yaw);
    void rotate_around(float angle, const glm::vec3 axis);

    const glm::vec3& get_pos() const
    {
        return m_eye;
    }

    const glm::mat4& get_view() const
    {
        return m_view;
    }
    const glm::mat4& get_projection() const
    {
        return m_projection;
    }

    void update_view();
    void update_projection();

private:
    glm::vec3 m_eye, m_center, m_up;
    float m_fovy, m_aspect, m_near, m_far;

    glm::mat4 m_view;
    glm::mat4 m_projection;
};
