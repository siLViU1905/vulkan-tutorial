#ifndef VULKANAPP_H
#define VULKANAPP_H
#include <array>
#include <string>
#include <vulkan/vulkan.h>

#include "../include/GLFW/glfw3.h"


class VulkanApp
{
    //static members
    static constexpr int WINDOW_WIDTH = 800;

    static constexpr int WINDOW_HEIGHT = 600;

    static constexpr std::array<const char*, 1> VALIDATION_LAYERS = {
        "VK_LAYER_KHRONOS_validation"
    };

    //private members
    GLFWwindow *m_Window;

    VkInstance m_Instance;

    //methods
    void enumerateAvailableExtensions();

    bool checkValidationLayerSupport();

    void createInstance();
public:
    VulkanApp();

    void run();

    ~VulkanApp();
};

#endif //VULKANAPP_H
