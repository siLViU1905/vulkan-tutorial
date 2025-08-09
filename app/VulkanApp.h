#ifndef VULKANAPP_H
#define VULKANAPP_H
#include "../include/GLFW/glfw3.h"


class VulkanApp
{
    static constexpr int WINDOW_WIDTH = 800;

    static constexpr int WINDOW_HEIGHT = 600;



   GLFWwindow* m_Window;
public:
    VulkanApp();

    void run();

    ~VulkanApp();
};

#endif //VULKANAPP_H
