#include "LightBuffer.h"

LightBuffer::LightBuffer() : m_Color(1.0f, 1.0f, 1.0f, 0.0f)
                             , m_Diffuse(1.0f, 1.0f, 1.0f, 0.0f)
                             , m_Specular(1.0f, 1.0f, 1.0f, 0.0f)
                             , m_Ambient(0.1f, 0.1f, 0.1f, 0.0f)
                             , m_Direction(0.0f, -1.0f, 0.0f, 0.0f)
{
}
