#ifndef UTILITY_H
#define UTILITY_H
#include <vulkan/vulkan.h>

namespace vkutl
{
    VkCommandBuffer beginSingleTimeCommands(const VkDevice &device, const VkCommandPool &commandPool);

    void endSingleTimeCommands(const VkDevice &device, const VkQueue &graphicsQueue, const VkCommandPool &commandPool,
                               VkCommandBuffer &commandBuffer);

    void copyBuffer(const VkDevice &device, const VkQueue &graphicsQueue, const VkCommandPool &commandPool,
                    const VkBuffer &src, VkBuffer &dstBuffer, VkDeviceSize size);

    uint32_t findMemoryType(const VkPhysicalDevice &physicalDevice, uint32_t typeFilter,
                            VkMemoryPropertyFlags properties);

    void createBuffer(const VkPhysicalDevice &physicalDevice, const VkDevice &device, VkDeviceSize size,
                      VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
                      VkBuffer &buffer, VkDeviceMemory &bufferMemory);
} // vk

#endif //UTILITY_H
