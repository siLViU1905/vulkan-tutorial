#version 460

layout(location = 0) in vec3 aPos;

layout(location = 0) out vec3 texCoords;


layout(binding = 0) uniform MVP
{
    mat4 model;
    mat4 view;
    mat4 projection;
} mvp;

void main()
{
    texCoords = aPos;

    mat4 viewNoTranslation = mat4(mat3(mvp.view));

    vec4 pos = mvp.projection * viewNoTranslation * vec4(aPos, 1.0);

    gl_Position = pos.xyww;
}