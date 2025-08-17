#include "LightBuffer.h"

LightBuffer::LightBuffer() : m_Color(1.0f, 1.0f, 1.0f)
                             , m_Diffuse(1.0f, 1.0f, 1.0f)
                             , m_Specular(1.0f, 1.0f, 1.0f)
                             , m_Ambient(0.1f, 0.1f, 0.1f)
                             , m_Position(0.0f, 1.f, 0.0f),
                             constant(1.f), linear(0.5f), quadratic(0.032f)
{
}
