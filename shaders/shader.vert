#version 460

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec2 aTexCoords;
layout(location = 4) in vec3 aTangent;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec3 fragPos;
layout(location = 2) out vec3 fragNormal;
layout(location = 3) out vec2 fragTexCoords;
layout(location = 4) out mat3 fragTBN;

layout(location = 7) out vec3 viewPos;

layout(binding = 0) uniform MVP
{
    mat4 model;
    mat4 view;
    mat4 projection;
}
mvp;

void main()
{
    fragColor = aColor;

    fragTexCoords = aTexCoords;

    mat3 normalMatrix = mat3(transpose(inverse(mvp.model)));

    vec3 N = normalize(normalMatrix * aNormal);

    vec3 T = normalize(normalMatrix * aTangent);

    T = normalize(T - dot(T, N) * N);

    vec3 B = cross(N, T);

    fragNormal = N;

    fragTBN = mat3(T, B, N);

    mat4 invView = inverse(mvp.view);

    viewPos = invView[3].xyz;

    fragPos = vec3(mvp.model * vec4(aPos, 1.0));

    gl_Position = mvp.projection * mvp.view * vec4(fragPos, 1.0);
}