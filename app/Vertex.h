#ifndef VERTEX_H
#define VERTEX_H
#include <vulkan/vulkan.h>
#include "../include/glm/glm.hpp"
#include <array>

struct Vertex
{
    glm::vec3 position;
    glm::vec3 color;
    glm::vec3 normal;
    glm::vec2 texCoords;

    static VkVertexInputBindingDescription getBindingDescription();

    static std::array<VkVertexInputAttributeDescription, 4> getAttributeDescriptions();
};

#endif //VERTEX_H
