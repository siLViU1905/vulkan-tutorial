#ifndef VULKANAPP_H
#define VULKANAPP_H
#include <array>
#include <optional>
#include <string>

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include "../include/GLFW/glfw3.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include <vector>

#include "../include/GLFW/glfw3native.h"

#include <vulkan/vulkan.h>

#include "Mesh.h"
#include "Vertex.h"


class VulkanApp
{
    //static members
    static constexpr int WINDOW_WIDTH = 800;

    static constexpr int WINDOW_HEIGHT = 600;

    static constexpr size_t MAX_FRAMES_IN_FLIGHT = 2;

    static constexpr std::array<const char *, 1> VALIDATION_LAYERS = {
        "VK_LAYER_KHRONOS_validation"
    };

    static constexpr std::array<const char *, 1> DEVICE_EXTENSIONS = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    inline static bool WINDOW_RESIZED = false;

     /*inline static std::vector<Vertex> vertices = {
        {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},

        {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
    };

    inline static std::vector<uint16_t> indices = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4
    };*/

    //static methods
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
        void *pUserData
    );

    static auto readFile(const std::string &path);

    static void framebufferCallback(GLFWwindow *window, int width, int height);


    //structs
    struct QueueFamilyIndices
    {
        std::optional<uint32_t> graphicsFamily;

        std::optional<uint32_t> presentFamily;

        bool isComplete();
    };

    struct SwapChainSupportDetails
    {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
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

    VkSwapchainKHR m_SwapChain;

    std::vector<VkImage> m_SwapChainImages;

    VkFormat m_SwapChainImageFormat;

    VkExtent2D m_SwapChainExtent;

    std::vector<VkImageView> m_SwapChainImageViews;

    VkRenderPass m_RenderPass;

    VkDescriptorSetLayout m_DescriptorSetLayout;

    VkPipelineLayout m_PipelineLayout;

    VkPipeline m_GraphicsPipeline;

    std::vector<VkFramebuffer> m_SwapChainFramebuffers;

    VkCommandPool m_CommandPool;

    std::vector<VkCommandBuffer> m_CommandBuffers;

    std::vector<VkSemaphore> m_ImageAvailableSemaphores;

    std::vector<VkSemaphore> m_RenderFinishedSemaphores;

    std::vector<VkFence> m_InFlightFences;

    uint32_t currentFrame;

    VkBuffer m_VertexBuffer;

    VkDeviceMemory m_VertexBufferMemory;

    VkBuffer m_IndexBuffer;

    VkDeviceMemory m_IndexBufferMemory;

    std::vector<VkBuffer> m_UniformBuffers;

    std::vector<VkDeviceMemory> m_UniformBuffersMemory;

    std::vector<void *> m_UniformBuffersMapped;

    VkDescriptorPool m_DescriptorPool;

    std::vector<VkDescriptorSet> m_DescriptorSets;

    uint32_t m_MipLevels;

    VkImage m_TextureImage;

    VkDeviceMemory m_TextureImageMemory;

    VkImageView m_TextureImageView;

    VkSampler m_TextureSampler;

    VkImage m_DepthImage;

    VkDeviceMemory m_DepthImageMemory;

    VkImageView m_DepthImageView;

    Mesh m_VikingRoom;

    VkSampleCountFlagBits m_MsaaSamples;

    VkImage m_ColorImage;

    VkDeviceMemory m_ColorImageMemory;

    VkImageView m_ColorImageView;

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

    bool checkDeviceExtensionSupport(VkPhysicalDevice device);

    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);

    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);

    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);

    void createSwapChain();

    void createImageViews();

    void createGraphicsPipeline();

    VkShaderModule createShaderModule(const std::vector<char> &code);

    void createRenderPass();

    void createFramebuffers();

    void createCommandPool();

    void createCommandBuffers();

    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

    void drawFrame();

    void createSyncObjects();

    void cleanupSwapChain();

    void recreateSwapChain();

    void createVertexBuffer();

    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &buffer,
                      VkDeviceMemory &bufferMemory);

    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    void copyBuffer(VkBuffer src, VkBuffer dstBuffer, VkDeviceSize size);

    void createIndexBuffer();

    void createDescriptorSetLayout();

    void createUniformBuffers();

    void updateUniformBuffer(uint32_t currentImage);

    void createDescriptorPool();

    void createDescriptorSets();

    void createTextureImage();

    void createImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling,
                     VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage &image,
                     VkDeviceMemory &memory);

    VkCommandBuffer beginSingleTimeCommands();

    void endSingleTimeCommands(VkCommandBuffer commandBuffer);

    void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);

    void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

    void createTextureImageView();

    VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);

    void createTextureSampler();

    void createDepthResources();

    VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

    VkFormat findDepthFormat();

    bool hasStencilComponent(VkFormat format);

    void generateMipmaps(VkImage image, VkFormat imageFormat, int32_t width, int32_t height, uint32_t mipLevels);

    VkSampleCountFlagBits getMaxUsableSampleCount();

    void createColorResources();

public:
    VulkanApp();

    void run();

    ~VulkanApp();
};
#endif //VULKANAPP_H
