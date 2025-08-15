#ifndef MESH_H
#define MESH_H
#include <string>
#include <vector>

#include "Vertex.h"

struct Mesh
{
    std::vector<Vertex> vertices;

    std::vector<uint32_t> indices;

    void load(const std::string& path);

private:
    void calculateTangents();
};


#endif //MESH_H
