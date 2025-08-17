#ifndef PBRMATERIALBUFFER_H
#define PBRMATERIALBUFFER_H
#include "../include/glm/glm.hpp"

struct PbrMaterialBuffer
{
    PbrMaterialBuffer();

    glm::vec4 m_Metalness;

    glm::vec4 m_Roughness;

    glm::vec4 m_Ao;
};


#endif //PBRMATERIALBUFFER_H
