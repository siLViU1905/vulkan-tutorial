#version 460

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoords;

layout (location = 0) out vec3 fragColor;
layout (location = 1) out vec3 fragPos;
layout (location = 2) out vec2 fragTexCoords;

layout (binding = 0) uniform MVP
{
    mat4 model;
    mat4 view;
    mat4 projection;
} mvp;


void main()
{
    fragColor = aColor;

    fragTexCoords = aTexCoords;

    fragPos = vec3(mvp.model * vec4(aPos, 0.0,1.0));

    gl_Position = mvp.projection * mvp.view * vec4(fragPos, 1.0);
}