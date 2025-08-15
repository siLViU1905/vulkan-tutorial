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
    glm::vec3 tangent;

    static VkVertexInputBindingDescription getBindingDescription();

    static std::array<VkVertexInputAttributeDescription, 5> getAttributeDescriptions();
};

#endif //VERTEX_H
