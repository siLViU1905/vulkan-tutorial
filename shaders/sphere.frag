#version 460

layout(location = 0) out vec4 outColor;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragPos;
layout(location = 2) in vec3 fragNormal;
layout(location = 3) in vec2 fragTexCoords;
layout(location = 4) in mat3 fragTBN;

layout(location = 7) in vec3 viewPos;

layout(binding = 1) uniform Light
{
    vec3 color;

    vec3 diffuse;

    vec3 specular;

    vec3 ambient;

    vec3 direction;
}
light;

vec3 processLight(
    vec3 materialColor,
    vec3 normal,
    float roughness,
    float ao,
    float specularIntensity
)
{
    vec3 ambient = light.ambient * materialColor * ao;

    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * light.color * diff * materialColor;

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    float shininess = mix(100.0, 5.0, roughness);

    float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
    vec3 specular = light.specular * spec * light.color * specularIntensity;

    return ambient + diffuse + specular;
}

void main()
{

    outColor = vec4(processLight(fragColor, fragNormal, 0.5, 0.5, 0.5), 1.0);
}