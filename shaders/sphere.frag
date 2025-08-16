#version 460

layout(location = 0) out vec4 outColor;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragPos;
layout(location = 2) in vec3 fragNormal;
layout(location = 3) in vec2 fragTexCoords;
layout(location = 4) in mat3 fragTBN;

layout(location = 7) in vec3 viewPos;

struct DirectionalLight
{
    vec3 direction;
    vec3 diffuse;
    vec3 specular;
    vec3 ambient;
    vec3 color;
};

vec3 processDirectionalLight(DirectionalLight light, vec3 materialColor, vec3 normal, float roughness, float ao,
                             float specularIntensity)
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

    DirectionalLight light;

    light.ambient = vec3(0.05);
    light.diffuse = vec3(0.3);
    light.specular = vec3(0.4);
    light.color = vec3(0.9);

    light.direction = vec3(-1.0, -1.0, 0.0);

    outColor = vec4(processDirectionalLight(light, fragColor,fragNormal,0.5,0.5,0.5),1.0);
}