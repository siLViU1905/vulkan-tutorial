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

    vec3 position;

    float constant;

    float linear;

    float quadratic;
}
light;

layout(binding = 2) uniform PbrMaterial
{
    vec4 albedo;

    float metalness;

    float roughness;

    float ao;
}
material;

layout(binding = 3) uniform samplerCube environmentMap;
layout(binding = 4) uniform sampler2D brdfLUT;

const float PI = 3.14159265359;

const float MAX_REFL_LOD = 4.0;

float distributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float nom = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

float geometrySchlickGGX(float NdotV, float roughness)
{
    float r = roughness + 1.0;
    float k = (r * r) / 8.0;

    float nom = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = geometrySchlickGGX(NdotV, roughness);
    float ggx1 = geometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness) 
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

void main()
{

    vec3 albedo = material.albedo.rgb;

    float metallic = material.metalness;

    float roughness = material.roughness;

    float ao = material.ao;


    vec3 N = normalize(fragNormal);

    vec3 V = normalize(viewPos - fragPos);

    vec3 R = reflect(-V, N);

    

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    vec3 Lo = vec3(0.0);

    vec3 L = normalize(light.position - fragPos);
    vec3 H = normalize(V + L);
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
        light.quadratic * (distance * distance));
    vec3 radiance = light.color * attenuation;

    float NDF = distributionGGX(N, H, roughness);
    float G = geometrySmith(N, V, L, roughness);
    vec3 F = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);

    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
    vec3 specular = numerator / denominator;

    vec3 kS = F;

    vec3 kD = vec3(1.0) - kS;

    kD *= 1.0 - metallic;

    float NdotL = max(dot(N, L), 0.0);

    Lo += (kD * albedo / PI + specular) * radiance * NdotL;

    vec3 F_ibl = fresnelSchlickRoughness(max(dot(N,V),0.0), F0, roughness);

    vec3 kS_ibl = F_ibl;

    vec3 kD_ibl = 1.0 - kS_ibl;

    kD_ibl *= 1.0 - metallic;

    vec3 irradiance = texture(environmentMap, N).rgb;

    vec3 diffuse_ibl = irradiance * albedo;

    vec3 prefilteredColor = textureLod(environmentMap, R, roughness * MAX_REFL_LOD).rgb;

    vec2 brdf = texture(brdfLUT, vec2(max(dot(N, V), 0.0), roughness)).rg;

    vec3 specular_ibl = prefilteredColor * (F_ibl * brdf.x + brdf.y);

    vec3 ambient = (kD_ibl * diffuse_ibl + specular_ibl) * ao;

    vec3 color = ambient + Lo;

    color = color / (color + vec3(1.0));

    color = pow(color, vec3(1.0 / 2.2));

    outColor = vec4(color, 1.0);
}