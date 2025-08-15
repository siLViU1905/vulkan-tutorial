#version 460

layout(location = 0) out vec4 outColor;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragPos;
layout(location = 2) in vec3 fragNormal;
layout(location = 3) in vec2 fragTexCoords;
layout(location = 4) in mat3 fragTBN;

layout(binding = 1) uniform sampler2D texSampler;
layout(binding = 2) uniform sampler2D normalSampler;

struct DirectionalLight
{
    vec3 direction;
    vec3 diffuse;
    vec3 specular;
    vec3 ambient;
    vec3 color;
};

vec3 processDirectionalLight(DirectionalLight light, vec3 materialColor, vec3 normal)
{
    vec3 ambient = light.ambient * materialColor;

    vec3 ViewPos = vec3(2.0);

    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * light.color * diff * materialColor;

    vec3 viewDir = normalize(ViewPos - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    vec3 specular = light.specular * spec * light.color;

    return ambient + diffuse + specular;
}

void main()
{
    vec4 materialColor = texture(texSampler, fragTexCoords);

    vec3 normalMap = texture(normalSampler, fragTexCoords).rgb;

    normalMap = normalize(normalMap * 2.0 - 1.0);

    vec3 worldNormal = normalize(fragTBN * normalMap);

    DirectionalLight light;

    light.ambient = vec3(0.2);

    light.diffuse = vec3(0.8);

    light.specular = vec3(1.0);

    light.color = vec3(1.0);

    light.direction = vec3(0.0, -1.0, 0.0);

    outColor = vec4(processDirectionalLight(light, materialColor.rgb, worldNormal), materialColor.a);
}