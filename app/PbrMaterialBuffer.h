#ifndef PBRMATERIALBUFFER_H
#define PBRMATERIALBUFFER_H
#include "../include/glm/glm.hpp"

struct PbrMaterialBuffer
{
    PbrMaterialBuffer();

    float m_Metalness;

    float m_Roughness;

    float m_Ao;

    float padding;
};


#endif //PBRMATERIALBUFFER_H
