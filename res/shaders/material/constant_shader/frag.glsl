#version 330 core

struct Material {
    vec3 albedo;
    float metallic;
    float roughness;
    float ao;
    vec3 emissive;
};

struct PointLight {
    vec3 position;
    vec3 color;
    float intensity;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    vec3 color;
    float intensity;
    float inner_cutoff;
    float outer_cutoff;
};

struct DirectionalLight {
    vec3 direction;
    vec3 color;
    float intensity;
};

struct AreaLight {
    vec3 position;
    vec3 normal;
    vec3 color;
    float intensity;
    float width;
    float height;
};

uniform Material u_material;
uniform vec3 u_cam_pos;

#define MAX_POINT_LIGHTS 4
uniform PointLight u_point_lights[MAX_POINT_LIGHTS];
uniform int u_num_point_lights;

#define MAX_SPOT_LIGHTS 4
uniform SpotLight u_spot_lights[MAX_SPOT_LIGHTS];
uniform int u_num_spot_lights;

#define MAX_DIRECTIONAL_LIGHTS 4
uniform DirectionalLight u_directional_lights[MAX_DIRECTIONAL_LIGHTS];
uniform int u_num_directional_lights;

#define MAX_AREA_LIGHTS 4
uniform AreaLight u_area_lights[MAX_AREA_LIGHTS];
uniform int u_num_area_lights;

in vec3 v_normal;
in vec3 v_position;

out vec4 fragColor;

vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

float geometrySchlickGGX(float NdotV, float k) {
    return NdotV / (NdotV * (1.0 - k) + k);
}

float geometrySmith(vec3 N, vec3 V, vec3 L, float k) {
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx1 = geometrySchlickGGX(NdotV, k);
    float ggx2 = geometrySchlickGGX(NdotL, k);
    return ggx1 * ggx2;
}

float distributionGGX(vec3 N, vec3 H, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;
    float denom = NdotH2 * (a2 - 1.0) + 1.0;
    return a2 / (3.14159265359 * denom * denom);
}

vec3 calculatePointLight(PointLight light, vec3 fragPos, vec3 normal, vec3 viewDir, float roughness, vec3 albedo) {
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float distance = length(light.position - fragPos);
    vec3 attenuation = vec3(1.0) / (light.intensity + distance * 0.01);
    vec3 diffuse = light.color * albedo * max(dot(normal, lightDir), 0.0);
    vec3 specular = vec3(0.0);
    if (dot(normal, halfwayDir) > 0.0) {
        vec3 spec = distributionGGX(normal, halfwayDir, roughness) * geometrySmith(normal, viewDir, lightDir, roughness) * fresnelSchlick(max(dot(halfwayDir, viewDir), 0.0), vec3(0.04, 0.04, 0.04));
        specular = light.color * spec * attenuation;
    }
    return diffuse + specular;
}

vec3 calculateSpotLight(SpotLight light, vec3 fragPos, vec3 normal, vec3 viewDir, float roughness, vec3 albedo) {
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float distance = length(light.position - fragPos);
    vec3 attenuation = vec3(1.0) / (light.intensity + distance * 0.01);

    float spot = dot(lightDir, normalize(-light.direction));
    float inner_cosine = cos(radians(light.inner_cutoff));
    float outer_cosine = cos(radians(light.outer_cutoff));
    float smoothstep = smoothstep(inner_cosine, outer_cosine, spot);

    vec3 diffuse = light.color * albedo * max(dot(normal, lightDir), 0.0) * attenuation * smoothstep;
    vec3 specular = vec3(0.0);
    if (dot(normal, halfwayDir) > 0.0) {
        vec3 spec = distributionGGX(normal, halfwayDir, roughness) * geometrySmith(normal, viewDir, lightDir, roughness) * fresnelSchlick(max(dot(halfwayDir, viewDir), 0.0), vec3(0.04));
        specular = light.color * spec * attenuation * smoothstep;
    }
    return diffuse + specular;
}

vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir, float roughness, vec3 albedo) {
    vec3 lightDir = normalize(-light.direction);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    vec3 diffuse = light.color * albedo * max(dot(normal, lightDir), 0.0);
    vec3 specular = vec3(0.0);
    if (dot(normal, halfwayDir) > 0.0) {
        vec3 spec = distributionGGX(normal, halfwayDir, roughness) * geometrySmith(normal, viewDir, lightDir, roughness) * fresnelSchlick(max(dot(halfwayDir, viewDir), 0.0), vec3(0.04));
    specular = light.color * spec;
    }
    return diffuse + specular;
}

vec3 calculateAreaLight(AreaLight light, vec3 fragPos, vec3 normal, vec3 viewDir, float roughness, vec3 albedo) {
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float distance = length(light.position - fragPos);
    vec3 attenuation = vec3(1.0) / (light.intensity + distance * 0.01);
    vec3 diffuse = light.color * albedo * max(dot(normal, lightDir), 0.0);
    vec3 specular = vec3(0.0);
    if (dot(normal, halfwayDir) > 0.0) {
        vec3 spec = distributionGGX(normal, halfwayDir, roughness) * geometrySmith(normal, viewDir, lightDir, roughness) * fresnelSchlick(max(dot(halfwayDir, viewDir), 0.0), vec3(0.04));
    specular = light.color * spec * attenuation;
    }
    vec3 area = vec3(1.0);
    vec3 U, V;
    if (abs(normal.x) > 0.5) {
    V = vec3(0.0, 1.0, 0.0);
    } else {
    V = vec3(1.0, 0.0, 0.0);
    }
    U = normalize(cross(V, normal));
    V = normalize(cross(normal, U));
    vec2 coord = vec2(dot(lightDir, U), dot(lightDir, V));
    coord = (coord + vec2(1.0)) / 2.0;
    float width = light.width / distance;
    float height = light.height / distance;
    float d = max(0.0, 1.0 - length(coord - vec2(0.5)) / (width + height));
    return (diffuse + specular) * d;
}

void main() {
    // calculate surface normal and view vector
    vec3 N = normalize(v_normal);
    vec3 V = normalize(u_cam_pos - v_position);

    // calculate reflection vector
    vec3 R = reflect(-V, N);

    // calculate fresnel factor
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, u_material.albedo, u_material.metallic);
    vec3 F = fresnelSchlick(max(dot(R, V), 0.0), F0);

    // calculate point lighting
    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);
    for (int i = 0; i < u_num_point_lights; i++) {
        PointLight pointLight = u_point_lights[i];
        vec3 light = calculatePointLight(pointLight, v_position, N, V, u_material.roughness, u_material.albedo);
        diffuse += light;
        specular += light;
    }

    // calculate spot lighting
    vec3 spotDiffuse = vec3(0.0);
    vec3 spotSpecular = vec3(0.0);
    for (int i = 0; i < u_num_spot_lights; i++) {
        SpotLight spotLight = u_spot_lights[i];
        vec3 light = calculateSpotLight(spotLight, v_position, N, V, u_material.roughness, u_material.albedo);
        spotDiffuse += light;
        spotSpecular += light;
    }

    // calculate directional lighting
    vec3 directionalDiffuse = vec3(0.0);
    vec3 directionalSpecular = vec3(0.0);
    for (int i = 0; i < u_num_directional_lights; i++) {
        DirectionalLight directionalLight = u_directional_lights[i];
        vec3 light = calculateDirectionalLight(directionalLight, N, V, u_material.roughness, u_material.albedo);
        directionalDiffuse += light;
        directionalSpecular += light;
    }

    // calculate area lighting
    vec3 area = vec3(0.0);
    for (int i = 0; i < u_num_area_lights; i++) {
        AreaLight areaLight = u_area_lights[i];
        vec3 light = calculateAreaLight(areaLight, v_position, N, V, u_material.roughness, u_material.albedo);
        area += light;
    }

    // calculate emissive color
    vec3 emissive = u_material.emissive;

    // calculate final color
    vec3 color = diffuse + specular * F + spotDiffuse + spotSpecular * F + directionalDiffuse + directionalSpecular * F + area + emissive;
    fragColor = vec4(color, 1.0);
}
