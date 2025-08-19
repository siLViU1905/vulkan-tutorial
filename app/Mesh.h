#ifndef MESH_H
#define MESH_H
#include <string>
#include <vector>

#include "Vertex.h"

class Mesh
{
public:
    Mesh();

    std::vector<Vertex> vertices;

    std::vector<uint32_t> indices;

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

    glm::vec3 m_Color;

private:
    void calculateTangents();

    glm::vec3 m_Position;

    glm::vec3 m_Rotation;

    glm::vec3 m_Scale;

    glm::mat4 m_Model;

    //std::unordered_map<Vertex, uint32_t> m_UniqueVertices;

    void updateModel();
};

#endif //MESH_H
