#ifndef VERTEX_H
#define VERTEX_H
#include <vulkan/vulkan.h>
#include "../include/glm/glm.hpp"
#include <array>

struct Vertex
{
    glm::vec2 position;
    glm::vec3 color;
    glm::vec2 texCoords;

    static VkVertexInputBindingDescription getBindingDescription();

    static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions();
};

#endif //VERTEX_H
