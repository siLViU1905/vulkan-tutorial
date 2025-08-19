#version 460

layout(location = 0) out vec4 outColor;

layout(location = 0) in vec3 texCoords;

layout(binding = 1) uniform samplerCube skybox;

void main()
{
    const float gamma = 2.2;
    vec3 color = texture(skybox, texCoords).rgb;
    
    color = pow(color, vec3(1.0/gamma));
    
    outColor = vec4(color, 1.0);
}