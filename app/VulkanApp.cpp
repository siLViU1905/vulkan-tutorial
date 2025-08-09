#include "VulkanApp.h"

#include <stdexcept>


VulkanApp::VulkanApp()
{
  if (!glfwInit())
    throw std::runtime_error("GLFW initialization failed");

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    m_Window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Vulkan App", nullptr, nullptr);

    if (!m_Window)
        throw std::runtime_error("Window initialization failed");


}

void VulkanApp::run()
{
    while (!glfwWindowShouldClose(m_Window))
    {
        glfwPollEvents();
    }
}

VulkanApp::~VulkanApp()
{
    glfwDestroyWindow(m_Window);

    glfwTerminate();
}