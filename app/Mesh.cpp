#include "Mesh.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "../include/tiny_obj_loader.h"
#include "../include/glm/ext/matrix_transform.hpp"

Mesh::Mesh()
{
    m_Position = m_Rotation = glm::vec3(0.f);

    m_Color = m_Scale = glm::vec3(1.f);

    m_Model = glm::mat4(1.f);
}

void Mesh::load(const std::string &path)
{
    tinyobj::attrib_t attrib;

    std::vector<tinyobj::shape_t> shapes;

    std::vector<tinyobj::material_t> materials;

    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str()))
        throw std::runtime_error(err);

    for (const auto &shape: shapes)
        for (const auto &index: shape.mesh.indices)
        {
            Vertex vertex;

            vertex.position = {
                attrib.vertices[3 * index.vertex_index],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };

            vertex.texCoords = {
                attrib.texcoords[2 * index.texcoord_index],
                attrib.texcoords[2 * index.texcoord_index + 1]
            };

            if (index.normal_index >= 0 && !attrib.normals.empty())
                vertex.normal = {
                    attrib.normals[3 * index.normal_index],
                    attrib.normals[3 * index.normal_index + 1],
                    attrib.normals[3 * index.normal_index + 2]
                };


            vertex.color = {1.0f, 1.0f, 1.0f};

            vertices.push_back(vertex);

            indices.push_back(indices.size());
        }

    calculateTangents();
}

void Mesh::generateSphere(float radius, int stacks, int slices)
{
    vertices.clear();

    indices.clear();

    for (int i = 0; i <= stacks; ++i)
    {
        float phi = 3.14159f * static_cast<float>(i) / static_cast<float>(stacks);
        float y = radius * std::cos(phi);
        float ringRadius = radius * std::sin(phi);

        for (int j = 0; j <= slices; ++j)
        {
            float theta = 2.0f * 3.14159f * static_cast<float>(j) / static_cast<float>(slices);

            Vertex vertex;


            vertex.position.x = ringRadius * std::cos(theta);
            vertex.position.y = y;
            vertex.position.z = ringRadius * std::sin(theta);


            glm::vec3 normal = glm::normalize(vertex.position);
            vertex.normal = normal;


            vertex.texCoords.x = static_cast<float>(j) / static_cast<float>(slices);
            vertex.texCoords.y = static_cast<float>(i) / static_cast<float>(stacks);


            glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
            glm::vec3 tangent = glm::cross(up, normal);

            if (glm::length(tangent) < 0.001f)
                tangent = glm::vec3(1.0f, 0.0f, 0.0f);
            else
                tangent = glm::normalize(tangent);


            vertex.tangent = tangent;
            vertex.color = glm::vec3(1.0f, 1.0f, 1.0f);

            vertices.push_back(vertex);
        }
    }

    for (int i = 0; i < stacks; ++i)
    {
        for (int j = 0; j < slices; ++j)
        {
            int first = i * (slices + 1) + j;
            int second = first + slices + 1;

            indices.push_back(first);
            indices.push_back(first + 1);
            indices.push_back(second);

            indices.push_back(second);
            indices.push_back(first + 1);
            indices.push_back(second + 1);
        }
    }
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

void Mesh::updateModel()
{
    m_Model = glm::mat4(1.f);

    m_Model = glm::translate(m_Model, m_Position);

    m_Model = glm::rotate(m_Model, glm::radians(m_Rotation.x), glm::vec3(1.f, 0.f, 0.f));

    m_Model = glm::rotate(m_Model, glm::radians(m_Rotation.y), glm::vec3(0.f, 1.f, 0.f));

    m_Model = glm::rotate(m_Model, glm::radians(m_Rotation.z), glm::vec3(0.f, 0.f, 1.f));

    m_Model = glm::scale(m_Model, m_Scale);
}

void Mesh::calculateTangents()
{
    for (auto &vertex: vertices)
        vertex.tangent = glm::vec3(0.f);

    for (size_t i = 0; i < indices.size(); i += 3)
    {
        uint32_t i0 = indices[i];
        uint32_t i1 = indices[i + 1];
        uint32_t i2 = indices[i + 2];

        Vertex &v0 = vertices[i0];
        Vertex &v1 = vertices[i1];
        Vertex &v2 = vertices[i2];

        glm::vec3 edge1 = v1.position - v0.position;
        glm::vec3 edge2 = v2.position - v0.position;

        glm::vec2 deltaUV1 = v1.texCoords - v0.texCoords;
        glm::vec2 deltaUV2 = v2.texCoords - v0.texCoords;

        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        glm::vec3 tangent;
        tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

        v0.tangent += tangent;
        v1.tangent += tangent;
        v2.tangent += tangent;
    }

    for (auto &vertex: vertices)
    {
        if (glm::length(vertex.tangent) > 0.0001f)
        {
            vertex.tangent = glm::normalize(vertex.tangent - vertex.normal * glm::dot(vertex.normal, vertex.tangent));
        } else
        {
            glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
            vertex.tangent = glm::normalize(glm::cross(up, vertex.normal));
            if (glm::length(vertex.tangent) < 0.001f)
            {
                vertex.tangent = glm::vec3(1.0f, 0.0f, 0.0f);
            }
        }
    }
}
