#version 460

layout(location = 0) out vec4 outColor;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragPos;
layout(location = 2) in vec3 fragNormal;
layout(location = 3) in vec2 fragTexCoords;
layout(location = 4) in mat3 fragTBN;

layout(location = 7) in vec3 viewPos;

layout(binding = 2) uniform sampler2D texSampler;
layout(binding = 3) uniform sampler2D normalSampler;
layout(binding = 4) uniform sampler2D roughnessSampler;
layout(binding = 5) uniform sampler2D aoSampler;
layout(binding = 6) uniform sampler2D specularSampler;

layout(binding = 1) uniform Light
{
    vec3 color;

    vec3 diffuse;

    vec3 specular;

    vec3 ambient;

    vec3 position;

    float constant;

    float linear;

    float quadratic;
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

    vec3 lightDir = normalize(light.position - fragPos);

    float distance = length(light.position - fragPos);


    float attenuation = 1.0 / (light.constant + light.linear * distance +
        light.quadratic * (distance * distance));

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * light.color * diff * materialColor * attenuation;

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    float shininess = mix(100.0, 5.0, roughness);

    float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
    vec3 specular = light.specular * spec * light.color * specularIntensity * attenuation;

    return ambient + diffuse + specular;
}

void main()
{
    vec4 materialColor = texture(texSampler, fragTexCoords);

    vec3 normalMap = texture(normalSampler, fragTexCoords).rgb;

    normalMap = normalize(normalMap * 2.0 - 1.0);

    vec3 worldNormal = normalize(fragTBN * normalMap);

    float roughness = texture(roughnessSampler, fragTexCoords).r;

    float ao = texture(aoSampler, fragTexCoords).r;

    float specularIntensity = texture(specularSampler, fragTexCoords).r;

    outColor = vec4(processLight(materialColor.rgb, worldNormal, roughness, ao, specularIntensity), materialColor.a);
}