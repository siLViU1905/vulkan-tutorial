#version 460

layout (location = 0) out vec4 outColor;

layout (location = 0) in vec3 fragColor;
layout (location = 1) in vec3 fragPos;
layout (location = 2) in vec2 fragTexCoords;

layout (binding = 1) uniform sampler2D texSampler;

void main()
{
    outColor = texture(texSampler, fragTexCoords);
}