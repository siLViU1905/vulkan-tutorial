#ifndef LIGHTBUFFER_H
#define LIGHTBUFFER_H
#include "../include/glm/glm.hpp"


struct LightBuffer
{
    LightBuffer();

    glm::vec3 m_Color;

    float padding0;

    glm::vec3 m_Diffuse;

    float padding1;

    glm::vec3 m_Specular;

    float padding2;

    glm::vec3 m_Ambient;

    float padding3;

    glm::vec3 m_Position;

    float constant;

    float linear;

    float quadratic;

    float padding5;
};

//static_assert(sizeof(LightBuffer) % 16 == 0, "Sizeof LightBuffer is not divisible by 16");


#endif //LIGHTBUFFER_H
