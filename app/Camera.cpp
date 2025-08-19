#include "Camera.h"


#include "../include/glm/gtc/matrix_transform.hpp"

Camera::Camera(GLFWwindow *window, const glm::vec3 &position, float speed): m_Window(window),
                                                                            m_Position(position),
                                                                            m_Speed(speed),
                                                                            m_Focused(false)
{
    m_Up = glm::vec3(0.f, 1.f, 0.f);

    m_Forward = glm::vec3(0.0f, 0.0f, -1.0f);

    m_Right = glm::normalize(glm::cross(m_Forward, m_Up));

    m_View = glm::lookAt(m_Position, m_Position + m_Forward, m_Up);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    glfwGetCursorPos(window, &m_LastX, &m_LastY);

    m_Yaw = -90.f;

    m_Pitch = 0.f;

    m_StartTime = std::chrono::high_resolution_clock::now();

    m_Projection = glm::perspective(glm::radians(45.f), static_cast<float>(width) / static_cast<float>(height), 0.1f,
                                    100.f);
}

void Camera::updateMovement()
{
    m_Right = glm::normalize(glm::cross(m_Forward, m_Up));

    float deltaTime = getElapsedTime();

    if (glfwGetKey(m_Window, GLFW_KEY_W))
        m_Position += m_Forward * deltaTime * m_Speed;

    if (glfwGetKey(m_Window, GLFW_KEY_S))
        m_Position -= m_Forward * deltaTime * m_Speed;

    if (glfwGetKey(m_Window, GLFW_KEY_A))
        m_Position -= m_Right * deltaTime * m_Speed;

    if (glfwGetKey(m_Window, GLFW_KEY_D))
        m_Position += m_Right * deltaTime * m_Speed;

    if (glfwGetKey(m_Window, GLFW_KEY_SPACE))
        m_Position += glm::vec3(0.f, deltaTime, 0.f) * m_Speed;

    if (glfwGetKey(m_Window, GLFW_KEY_LEFT_CONTROL))
        m_Position -= glm::vec3(0.f, deltaTime, 0.f) * m_Speed;

    m_View = glm::lookAt(m_Position,
                         m_Position + m_Forward,
                         m_Up);
}

void Camera::updateLookAt()
{
    if (!m_Focused)
        return;

    double newX, newY;

    glfwGetCursorPos(m_Window, &newX, &newY);

    float xoffset = static_cast<float>(newX - m_LastX) * 0.1f;

    float yoffset = static_cast<float>(m_LastY - newY) * 0.1f;

    m_Yaw += xoffset;

    m_Pitch += yoffset;

    if (m_Pitch > 89.0f)
        m_Pitch = 89.0f;
    if (m_Pitch < -89.0f)
        m_Pitch = -89.0f;

    m_Yaw = std::fmod(m_Yaw, 360.0f);

    if (m_Yaw < 0.0f)
        m_Yaw += 360.0f;

    m_LastX = newX;
    m_LastY = newY;

    glm::vec3 direction;

    direction.x = std::cos(glm::radians(m_Yaw)) * std::cos(glm::radians(m_Pitch));
    direction.y = std::sin(glm::radians(m_Pitch));
    direction.z = std::sin(glm::radians(m_Yaw)) * std::cos(glm::radians(m_Pitch));

    m_Forward = glm::normalize(direction);

    m_Right = glm::normalize(glm::cross(m_Forward, m_Up));

    m_View = glm::lookAt(m_Position,
                         m_Position + m_Forward,
                         m_Up);
}

void Camera::updateProjection()
{
    int width, height;

    glfwGetFramebufferSize(m_Window, &width, &height);

    m_Projection = glm::perspective(glm::radians(45.f), static_cast<float>(width) / static_cast<float>(height), 0.1f,
                                    100.f);
}

void Camera::gainFocus()
{
    m_Focused = true;
}

void Camera::loseFocus()
{
    m_Focused = false;
}

float Camera::getElapsedTime()
{
    auto now = std::chrono::high_resolution_clock::now();

    std::chrono::duration<float> duration = now - m_StartTime;

    m_StartTime = now;

    return duration.count();
}
