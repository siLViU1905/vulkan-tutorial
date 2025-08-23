#ifndef VERTEX_H
#define VERTEX_H
#include <vulkan/vulkan.h>
#include "../include/glm/glm.hpp"
#include <array>

#define GLM_ENABLE_EXPERIMENTAL
#include "../include/glm/gtx/hash.hpp"

struct Vertex
{
    glm::vec3 m_Position;

    glm::vec3 m_Normal;

    glm::vec2 m_TexCoords;

    glm::vec3 m_Tangent;

    static VkVertexInputBindingDescription getBindingDescription();

    static std::array<VkVertexInputAttributeDescription, 4> getAttributeDescriptions();

    bool operator==(const Vertex &other) const;
};

namespace std
{
    template<>
    struct hash<Vertex>
    {
        size_t operator()(Vertex const& vertex) const noexcept
        {
            return ((hash<glm::vec3>()(vertex.m_Position) ^
                   (hash<glm::vec2>()(vertex.m_TexCoords) << 1)) >> 1) ^
                   (hash<glm::vec3>()(vertex.m_Normal) << 1);
        }
    };
}

#endif //VERTEX_H
