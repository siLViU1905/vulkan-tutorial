#ifndef VULKANAPP_H
#define VULKANAPP_H
#include <array>
#include <optional>
#include <string>

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include "../include/GLFW/glfw3.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include "../include/GLFW/glfw3native.h"

#include <vulkan/vulkan.h>


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

        std::optional<uint32_t> presentFamily;

        bool isComplete();
    };

    //private members
    GLFWwindow *m_Window;

    VkInstance m_Instance;

    VkDebugUtilsMessengerEXT m_DebugMessenger;

    VkPhysicalDevice m_PhysicalDevice;

    VkDevice m_Device;

    VkQueue m_GraphicsQueue;

    VkSurfaceKHR m_Surface;

    VkQueue m_PresentQueue;

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

    void createSurface();

public:
    VulkanApp();

    void run();

    ~VulkanApp();
};

#endif //VULKANAPP_H
