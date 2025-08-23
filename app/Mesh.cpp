#include "Mesh.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "../include/tiny_obj_loader.h"
#include "../include/glm/ext/matrix_transform.hpp"
#include "Utility.h"

Mesh::Mesh(): device(nullptr), graphicsQueue(nullptr), commandPool(nullptr), physicalDevice(nullptr),
              m_BuffersCreated(false)
{
    m_Position = m_Rotation = glm::vec3(0.f);

    m_Color = m_Scale = glm::vec3(1.f);

    m_Model = glm::mat4(1.f);
}

Mesh::Mesh(VkDevice &device, VkQueue &graphicsQueue, VkCommandPool &commandPool,
           VkPhysicalDevice &physicalDevice): device(&device),
                                              graphicsQueue(&graphicsQueue), commandPool(&commandPool),
                                              physicalDevice(&physicalDevice), m_BuffersCreated(false)
{
    m_Position = m_Rotation = glm::vec3(0.f);

    m_Color = m_Scale = glm::vec3(1.f);

    m_Model = glm::mat4(1.f);
}

void Mesh::load(const std::string &path)
{
    Assimp::Importer import;

    const aiScene* scene = import.ReadFile(
        path.c_str(),
        aiProcess_Triangulate | aiProcess_GenSmoothNormals |
        aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
        !scene->mRootNode)
        throw std::runtime_error("Failed to load model");

    processNode(scene->mRootNode, scene);

    m_UniqueVertices.clear();

    createBuffers();
}

void Mesh::generateSphere(float radius, int stacks, int slices)
{
    m_Vertices.clear();

    m_Indices.clear();

    for (int i = 0; i <= stacks; ++i)
    {
        float phi = 3.14159f * static_cast<float>(i) / static_cast<float>(stacks);
        float y = radius * std::cos(phi);
        float ringRadius = radius * std::sin(phi);

        for (int j = 0; j <= slices; ++j)
        {
            float theta = 2.0f * 3.14159f * static_cast<float>(j) / static_cast<float>(slices);

            Vertex vertex;


            vertex.m_Position.x = ringRadius * std::cos(theta);
            vertex.m_Position.y = y;
            vertex.m_Position.z = ringRadius * std::sin(theta);


            glm::vec3 normal = glm::normalize(vertex.m_Position);
            vertex.m_Normal = normal;


            vertex.m_TexCoords.x = static_cast<float>(j) / static_cast<float>(slices);
            vertex.m_TexCoords.y = static_cast<float>(i) / static_cast<float>(stacks);


            glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
            glm::vec3 tangent = glm::cross(up, normal);

            if (glm::length(tangent) < 0.001f)
                tangent = glm::vec3(1.0f, 0.0f, 0.0f);
            else
                tangent = glm::normalize(tangent);


            vertex.m_Tangent = tangent;

            m_Vertices.push_back(vertex);
        }
    }

    for (int i = 0; i < stacks; ++i)
    {
        for (int j = 0; j < slices; ++j)
        {
            int first = i * (slices + 1) + j;
            int second = first + slices + 1;

            m_Indices.push_back(first);
            m_Indices.push_back(first + 1);
            m_Indices.push_back(second);

            m_Indices.push_back(second);
            m_Indices.push_back(first + 1);
            m_Indices.push_back(second + 1);
        }
    }

    createBuffers();
}

void Mesh::move(const glm::vec3 &offset)
{
    m_Position += offset;

    updateModel();
}

void Mesh::rotate(const glm::vec3 &offset)
{
    m_Rotation += offset;

    updateModel();
}

void Mesh::scale(const glm::vec3 &offset)
{
    m_Scale += offset;

    updateModel();
}

void Mesh::setPosition(const glm::vec3 &position)
{
    m_Position = position;

    updateModel();
}

void Mesh::setRotation(const glm::vec3 &rotation)
{
    m_Rotation = rotation;

    updateModel();
}

void Mesh::setScale(const glm::vec3 &scale)
{
    m_Scale = scale;

    updateModel();
}

void Mesh::setDevice(VkDevice &device)
{
    this->device = &device;
}

void Mesh::setGraphicsQueue(VkQueue &graphicsQueue)
{
    this->graphicsQueue = &graphicsQueue;
}

void Mesh::setCommandPool(VkCommandPool &commandPool)
{
    this->commandPool = &commandPool;
}

void Mesh::setPhysicalDevice(VkPhysicalDevice &physicalDevice)
{
    this->physicalDevice = &physicalDevice;
}

void Mesh::setVertices(const std::vector<Vertex> &vertices)
{
    m_Vertices.clear();

    m_Vertices = vertices;
}

void Mesh::setIndices(const std::vector<uint32_t> &indices)
{
    m_Indices.clear();

    m_Indices = indices;
}

void Mesh::createBuffers()
{
    createVertexBuffer();

    createIndexBuffer();

    m_BuffersCreated = true;
}

void Mesh::cleanBuffers()
{
    if (!m_BuffersCreated)
        return;

    m_BuffersCreated = false;

    vkDestroyBuffer(*device, m_VertexBuffer, nullptr);

    vkFreeMemory(*device, m_VertexBufferMemory, nullptr);

    vkDestroyBuffer(*device, m_IndexBuffer, nullptr);

    vkFreeMemory(*device, m_IndexBufferMemory, nullptr);
}

void Mesh::updateModel()
{
    m_Model = glm::mat4(1.f);

    m_Model = glm::translate(m_Model, m_Position);

    m_Model = glm::rotate(m_Model, glm::radians(m_Rotation.x), glm::vec3(1.f, 0.f, 0.f));

    m_Model = glm::rotate(m_Model, glm::radians(m_Rotation.y), glm::vec3(0.f, 1.f, 0.f));

    m_Model = glm::rotate(m_Model, glm::radians(m_Rotation.z), glm::vec3(0.f, 0.f, 1.f));

    m_Model = glm::scale(m_Model, m_Scale);
}

void Mesh::createVertexBuffer()
{
    if (m_Vertices.empty())
        throw std::runtime_error("Vertices are empty");

    VkDeviceSize bufferSize = sizeof(m_Vertices[0]) * m_Vertices.size();

    VkBuffer stagingBuffer;

    VkDeviceMemory stagingBufferMemory;

    vkutl::createBuffer(*physicalDevice, *device, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                        stagingBuffer, stagingBufferMemory);

    void *data;

    vkMapMemory(*device, stagingBufferMemory, 0, bufferSize, 0, &data);

    memcpy(data, m_Vertices.data(), bufferSize);

    vkUnmapMemory(*device, stagingBufferMemory);

    vkutl::createBuffer(*physicalDevice, *device,
                        bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                        m_VertexBuffer, m_VertexBufferMemory);

    vkutl::copyBuffer(*device, *graphicsQueue, *commandPool,
                      stagingBuffer, m_VertexBuffer, bufferSize);

    vkDestroyBuffer(*device, stagingBuffer, nullptr);

    vkFreeMemory(*device, stagingBufferMemory, nullptr);
}

void Mesh::createIndexBuffer()
{
    if (m_Indices.empty())
        throw std::runtime_error("Indices are empty");

    VkDeviceSize bufferSize = sizeof(m_Indices[0]) * m_Indices.size();

    VkBuffer stagingBuffer;

    VkDeviceMemory stagingBufferMemory;

    vkutl::createBuffer(*physicalDevice, *device, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                        stagingBuffer, stagingBufferMemory);

    void *data;

    vkMapMemory(*device, stagingBufferMemory, 0, bufferSize, 0, &data);

    memcpy(data, m_Indices.data(), bufferSize);

    vkUnmapMemory(*device, stagingBufferMemory);

    vkutl::createBuffer(*physicalDevice, *device,
                        bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                        m_IndexBuffer, m_IndexBufferMemory);

    vkutl::copyBuffer(*device, *graphicsQueue, *commandPool,
                      stagingBuffer, m_IndexBuffer, bufferSize);

    vkDestroyBuffer(*device, stagingBuffer, nullptr);

    vkFreeMemory(*device, stagingBufferMemory, nullptr);
}

void Mesh::processNode(aiNode *node, const aiScene *scene)
{
    for (uint32_t i = 0; i < node->mNumMeshes; ++i)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];

        processMesh(mesh, scene);
    }

    for (uint32_t i = 0; i < node->mNumChildren; ++i)
        processNode(node->mChildren[i], scene);
}

void Mesh::processMesh(aiMesh *mesh, const aiScene *scene)
{
    for (uint32_t i = 0; i < mesh->mNumFaces; ++i)
    {
        const aiFace& face = mesh->mFaces[i];

        for (uint32_t j = 0; j < face.mNumIndices; ++j)
        {
            uint32_t vertexIndex = face.mIndices[j];

            Vertex vertex;

            vertex.m_Position = glm::vec3(mesh->mVertices[vertexIndex].x,
                                          mesh->mVertices[vertexIndex].y,
                                          mesh->mVertices[vertexIndex].z);

            if (mesh->HasNormals())
                vertex.m_Normal = glm::vec3(mesh->mNormals[vertexIndex].x,
                                            mesh->mNormals[vertexIndex].y,
                                            mesh->mNormals[vertexIndex].z);


            if (mesh->mTextureCoords[0])
                vertex.m_TexCoords = glm::vec2(mesh->mTextureCoords[0][vertexIndex].x,
                                               mesh->mTextureCoords[0][vertexIndex].y);


            if (mesh->HasTangentsAndBitangents()) {
                vertex.m_Tangent = glm::vec3(mesh->mTangents[vertexIndex].x,
                                             mesh->mTangents[vertexIndex].y,
                                             mesh->mTangents[vertexIndex].z);
            }

            if (!m_UniqueVertices.contains(vertex))
            {
                m_UniqueVertices[vertex] = static_cast<uint32_t>(m_Vertices.size());
                m_Vertices.push_back(vertex);
            }

            m_Indices.push_back(m_UniqueVertices[vertex]);
        }
    }
}

void Mesh::calculateTangents()
{
    for (auto &vertex: m_Vertices)
        vertex.m_Tangent = glm::vec3(0.f);

    for (size_t i = 0; i < m_Indices.size(); i += 3)
    {
        uint32_t i0 = m_Indices[i];
        uint32_t i1 = m_Indices[i + 1];
        uint32_t i2 = m_Indices[i + 2];

        Vertex &v0 = m_Vertices[i0];
        Vertex &v1 = m_Vertices[i1];
        Vertex &v2 = m_Vertices[i2];

        glm::vec3 edge1 = v1.m_Position - v0.m_Position;
        glm::vec3 edge2 = v2.m_Position - v0.m_Position;

        glm::vec2 deltaUV1 = v1.m_TexCoords - v0.m_TexCoords;
        glm::vec2 deltaUV2 = v2.m_TexCoords - v0.m_TexCoords;

        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        glm::vec3 tangent;
        tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

        v0.m_Tangent += tangent;
        v1.m_Tangent += tangent;
        v2.m_Tangent += tangent;
    }

    for (auto &vertex: m_Vertices)
    {
        if (glm::length(vertex.m_Tangent) > 0.0001f)
        {
            vertex.m_Tangent = glm::normalize(
                vertex.m_Tangent - vertex.m_Normal * glm::dot(vertex.m_Normal, vertex.m_Tangent));
        } else
        {
            glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
            vertex.m_Tangent = glm::normalize(glm::cross(up, vertex.m_Normal));
            if (glm::length(vertex.m_Tangent) < 0.001f)
            {
                vertex.m_Tangent = glm::vec3(1.0f, 0.0f, 0.0f);
            }
        }
    }
}

Mesh::~Mesh()
{
    cleanBuffers();
}
