#ifndef TEXTURE_H
#define TEXTURE_H
#include <vulkan/vulkan.h>

struct Texture
{
    VkImage image;

    VkImageView imageView;

    VkDeviceMemory memory;

    VkSampler sampler;

    uint32_t mipLevels;
};

#endif //TEXTURE_H
