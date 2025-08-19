#ifndef CAMERA_H
#define CAMERA_H
#include <chrono>

#include "../include/GLFW/glfw3.h"
#include "../include/glm/glm.hpp"

struct Camera
{
    Camera(GLFWwindow* window, const glm::vec3& position, float speed);

    glm::mat4 m_View;

    glm::mat4 m_Projection;

    void updateMovement();

    void updateLookAt();

    void updateProjection();

    void gainFocus();

    void loseFocus();

private:
    glm::vec3 m_Position;

    glm::vec3 m_Forward;

    glm::vec3 m_Right;

    glm::vec3 m_Up;

    float m_Yaw;

    float m_Pitch;

    float m_Speed;

    std::chrono::high_resolution_clock::time_point m_StartTime;

    double m_LastX;

    double m_LastY;

    bool m_Focused;

    GLFWwindow* m_Window;

    float getElapsedTime();
};



#endif //CAMERA_H
