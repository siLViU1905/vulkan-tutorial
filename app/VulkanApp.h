#ifndef VULKANAPP_H
#define VULKANAPP_H
#include <array>
#include <optional>
#include <string>
#include <vulkan/vulkan.h>

#include "../include/GLFW/glfw3.h"


class VulkanApp
{
    //static members
    static constexpr int WINDOW_WIDTH = 800;

    static constexpr int WINDOW_HEIGHT = 600;

    static constexpr std::array<const char *, 1> VALIDATION_LAYERS = {
        "VK_LAYER_KHRONOS_validation"
    };

    //static methods
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
        void *pUserData
    );

    //structs
    struct QueueFamilyIndices
    {
        std::optional<uint32_t> graphicsFamily;

        bool isComplete();
    };

    //private members
    GLFWwindow *m_Window;

    VkInstance m_Instance;

    VkDebugUtilsMessengerEXT m_DebugMessenger;

    VkPhysicalDevice m_PhysicalDevice;

    VkDevice m_Device;

    VkQueue m_GraphicsQueue;

    //methods
    void enumerateAvailableExtensions();

    bool checkValidationLayerSupport();

    auto getRequiredExtensions();

    void createInstance();

    void setupDebugMessenger();

    VkResult createDebugUtilsMessengerEXT(VkInstance instance,
                                          const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
                                          const VkAllocationCallbacks *pAllocator,
                                          VkDebugUtilsMessengerEXT *pDebugMessenger);

    void destroyDebugUtilsMessengerEXT(VkInstance instance,
                                       VkDebugUtilsMessengerEXT debugMessenger,
                                       const VkAllocationCallbacks *pAllocator);

    bool isDeviceSuitable(VkPhysicalDevice device);

    int rateDevice(VkPhysicalDevice device);

    void pickPhysicalDevice();

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

    void createLogicalDevice();

public:
    VulkanApp();

    void run();

    ~VulkanApp();
};

#endif //VULKANAPP_H
