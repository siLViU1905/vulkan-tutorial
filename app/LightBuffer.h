#ifndef LIGHTBUFFER_H
#define LIGHTBUFFER_H
#include "../include/glm/glm.hpp"


struct LightBuffer
{
    LightBuffer();

    glm::vec4 m_Color;

    glm::vec4 m_Diffuse;

    glm::vec4 m_Specular;

    glm::vec4 m_Ambient;

    glm::vec4 m_Direction;
};

static_assert(sizeof(LightBuffer) % 16 == 0, "Sizeof LightBuffer is not divisible by 16");


#endif //LIGHTBUFFER_H
