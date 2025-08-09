#include "VulkanApp.h"

#include <iostream>
#include <map>
#include <stdexcept>
#include <vector>


VkBool32 VulkanApp::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                  VkDebugUtilsMessageTypeFlagsEXT messageType,
                                  const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                                  void *pUserData)
{
    //if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
        std::cout << "Validation layer: " << pCallbackData->pMessage << '\n';

    return VK_FALSE;
}

void VulkanApp::enumerateAvailableExtensions()
{
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> extensions(extensionCount);

    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

    std::cout << "Available extensions:\n";

    for (const auto &extension: extensions)
        std::cout << '\t' << extension.extensionName << '\n';
}

bool VulkanApp::checkValidationLayerSupport()
{
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const auto &layerName: VALIDATION_LAYERS)
    {
        bool layerFound = false;

        for (const auto &layerProperties: availableLayers)
        {
            if (!strcmp(layerName, layerProperties.layerName))
            {
                layerFound = true;
                break;
            }
        }

        if (!layerFound)
            return false;
    }

    return true;
}

auto VulkanApp::getRequiredExtensions()
{
    uint32_t glfwExtensionCount = 0;
    const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

    return extensions;
}

void VulkanApp::createInstance()
{
    if (!checkValidationLayerSupport())
        throw std::runtime_error("Validation layers requested, but not available");

    VkApplicationInfo appInfo{};

    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Vulkan App";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_4;

    VkInstanceCreateInfo createInfo{};

    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    uint32_t glfwExtensionCount = 0;
    const char **glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;

    createInfo.enabledLayerCount = static_cast<uint32_t>(VALIDATION_LAYERS.size());
    createInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();

    auto extensions = getRequiredExtensions();

    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    if (vkCreateInstance(&createInfo, nullptr, &m_Instance))
        throw std::runtime_error("Failed to create instance");
}

void VulkanApp::setupDebugMessenger()
{
    VkDebugUtilsMessengerCreateInfoEXT createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                             VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                             VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;

    if (createDebugUtilsMessengerEXT(m_Instance, &createInfo, nullptr, &m_DebugMessenger))
        throw std::runtime_error("Failed to set up debug messenger");
}

VkResult VulkanApp::createDebugUtilsMessengerEXT(VkInstance instance,
                                                 const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
                                                 const VkAllocationCallbacks *pAllocator,
                                                 VkDebugUtilsMessengerEXT *pDebugMessenger)
{
    auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
        vkGetInstanceProcAddr(m_Instance, "vkCreateDebugUtilsMessengerEXT"));

    if (func)
        return func(m_Instance, pCreateInfo, pAllocator, pDebugMessenger);

    return VK_ERROR_EXTENSION_NOT_PRESENT;
}

void VulkanApp::destroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
                                              const VkAllocationCallbacks *pAllocator)
{
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)
            vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func)
        func(instance, debugMessenger, pAllocator);
}

bool VulkanApp::isDeviceSuitable(VkPhysicalDevice device)
{
    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
           deviceFeatures.geometryShader;
}

int VulkanApp::rateDevice(VkPhysicalDevice device)
{
    int score = 0;

    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        score += 1000;

    score += deviceProperties.limits.maxImageDimension2D;

    if (!deviceFeatures.geometryShader)
        return 0;

    return score;
}

void VulkanApp::pickPhysicalDevice()
{
    m_PhysicalDevice = VK_NULL_HANDLE;

    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(m_Instance, &deviceCount, nullptr);

    if (!deviceCount)
        throw std::runtime_error("Failed to find GPUs with Vulkan support");

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(m_Instance, &deviceCount, devices.data());

    std::multimap<int, VkPhysicalDevice> candidates;

    for (const auto& device : devices) {
        int score = rateDevice(device);
        candidates.insert(std::make_pair(score, device));
    }

    if (candidates.rbegin()->first > 0)
        m_PhysicalDevice = candidates.rbegin()->second;
     else
        throw std::runtime_error("Failed to find a suitable GPU");

}

VulkanApp::VulkanApp()
{
    if (!glfwInit())
        throw std::runtime_error("GLFW initialization failed");

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    m_Window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Vulkan App", nullptr, nullptr);

    if (!m_Window)
        throw std::runtime_error("Window initialization failed");

    enumerateAvailableExtensions();

    createInstance();

    setupDebugMessenger();

    pickPhysicalDevice();
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
    destroyDebugUtilsMessengerEXT(m_Instance, m_DebugMessenger, nullptr);

    vkDestroyInstance(m_Instance, nullptr);

    glfwDestroyWindow(m_Window);

    glfwTerminate();
}
