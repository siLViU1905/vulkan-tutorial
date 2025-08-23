#ifndef MESH_H
#define MESH_H
#include <string>
#include <vector>

#include "Vertex.h"
#include "../include/assimp/Importer.hpp"
#include "../include/assimp/postprocess.h"
#include "../include/assimp/scene.h"


class Mesh
{
public:
    Mesh();

    Mesh(VkDevice& device, VkQueue& graphicsQueue, VkCommandPool& commandPool, VkPhysicalDevice& physicalDevice);

    void load(const std::string &path);

    void generateSphere(float radius, int stacks, int slices);

    void move(const glm::vec3 &offset);

    void rotate(const glm::vec3 &offset);

    void scale(const glm::vec3 &offset);

    constexpr const glm::vec3 &getPosition() const
    {
        return m_Position;
    }

    void setPosition(const glm::vec3 &position);

    constexpr const glm::vec3 &getRotation() const
    {
        return m_Rotation;
    }

    void setRotation(const glm::vec3 &rotation);

    constexpr const glm::vec3 &getScale() const
    {
        return m_Scale;
    }

    void setScale(const glm::vec3 &scale);

    constexpr const auto &getModel() const { return m_Model; }

    constexpr const auto& getVertices() const {return m_Vertices;}

    constexpr const auto& getIndices() const {return m_Indices;}

    constexpr const VkBuffer& getVertexBuffer() const{return m_VertexBuffer;}

    constexpr const VkBuffer& getIndexBuffer() const{return m_IndexBuffer;}

    void setDevice(VkDevice& device);

    void setGraphicsQueue(VkQueue& graphicsQueue);

    void setCommandPool(VkCommandPool& commandPool);

    void setPhysicalDevice(VkPhysicalDevice& physicalDevice);

    void setVertices(const std::vector<Vertex>& vertices);

    void setIndices(const std::vector<uint32_t>& indices);

    void createBuffers();

    void cleanBuffers();

    ~Mesh();

    glm::vec3 m_Color;

private:
    void calculateTangents();

    glm::vec3 m_Position;

    glm::vec3 m_Rotation;

    glm::vec3 m_Scale;

    glm::mat4 m_Model;

    std::vector<Vertex> m_Vertices;

    std::vector<uint32_t> m_Indices;

    std::unordered_map<Vertex, uint32_t> m_UniqueVertices;

    void updateModel();

    VkBuffer m_VertexBuffer;

    VkDeviceMemory m_VertexBufferMemory;

    VkBuffer m_IndexBuffer;

    VkDeviceMemory m_IndexBufferMemory;

    bool m_BuffersCreated;

    VkDevice* device;

     VkQueue* graphicsQueue;

     VkCommandPool* commandPool;

    VkPhysicalDevice* physicalDevice;

    void createVertexBuffer();

    void createIndexBuffer();

    void processNode(aiNode* node, const aiScene* scene);

    void processMesh(aiMesh* mesh, const aiScene* scene);
};
#endif //MESH_H
