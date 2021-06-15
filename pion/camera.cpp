#include "pch.h"
#include "camera.h"

camera::camera()
{
    m_eye = glm::vec3(0.0f, 2.0f, 3.0f);
    m_center = glm::vec3(0);
    m_up = glm::normalize(glm::vec3(0.0f, 3.0f, -2.0f));

    m_aspect = glm::radians(100.0f);
    m_fovy = 4.0f / 3.0f;
    m_near = 0.1f;
    m_far = 100.0f;

    update_view();
    update_projection();
}

void camera::move(float dx, float dy, float dz)
{
    glm::vec3 front = glm::normalize(m_center - m_eye);
    glm::vec3 right = glm::normalize(glm::cross(front, m_up));
    glm::vec3 dp = dx * right + dy * front - dz * m_up;

    m_eye += dp;
    m_center += dp;
}

void camera::rotate_self(float pitch, float yaw)
{
    glm::mat3 m1 = glm::mat3(glm::rotate(glm::mat4(1.0f), pitch, glm::vec3(1.0f, 0.0f, 0.0f)));
    glm::mat3 m2 = glm::mat3(glm::rotate(glm::mat4(1.0f), yaw, glm::vec3(0.0f, 1.0f, 0.0f)));
    glm::mat3 m = m1 * m2;

    glm::vec3 front = m * (m_center - m_eye);

    m_center = m_eye + front;
}

void camera::rotate_around(float angle, const glm::vec3 axis)
{
    glm::mat3 m = glm::mat3(glm::rotate(glm::mat4(1.0f), angle, axis));

    m_eye = m * (m_eye - m_center) + m_center;
    m_up = m * m_up;
}

void camera::update_view()
{
    m_view = glm::lookAt(m_eye, m_center, m_up);
}

void camera::update_projection()
{
    m_projection = glm::perspective(m_fovy, m_aspect, m_near, m_far);
}
