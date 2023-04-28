#version 330 core

#define TEXTURE_COLORS

out vec4 fragColor;
in vec3 v_normal;
in vec3 v_position;
in vec2 v_tex_coords;

#ifdef CONSTANT_COLORS
struct Material {
    vec3 albedo;
    float metallic;
    float roughness;
    float ao;
    vec3 emissive;
};
#endif

#ifdef TEXTURE_COLORS
struct Material {
    sampler2D albedoMap;
    sampler2D metallicMap;
    sampler2D roughnessMap;
    sampler2D aoMap;
    sampler2D emissiveMap;
};
#endif

#ifdef NORMAL_MAP
uniform sampler2D normalMap;
#endif

struct PointLight {
    vec3 position;
    vec3 color;
    float intensity;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    vec3 color;
    vec3 intensity;

    float cutoff;
    float outer_cutoff;
    float constant;
    float linear;
    float quadratic;
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

uniform vec3 u_cam_pos;

const float PI = 3.14159265359;
  
float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 fresnelSchlick(float cosTheta, vec3 F0);
#ifdef NORMAL_MAP
vec3 getNormalFromMap();
#endif

void main()
{
    #ifdef TEXTURE_COLORS
    vec3 albedo     = pow(texture(u_material.albedoMap, v_tex_coords).rgb, vec3(2.2));
    float metallic  = texture(u_material.metallicMap, v_tex_coords).r;
    float roughness = texture(u_material.roughnessMap, v_tex_coords).r;
    float ao        = texture(u_material.aoMap, v_tex_coords).r;
    vec3 emissive   = pow(texture(u_material.emissiveMap, v_tex_coords).rgb, vec3(2.2));
    #endif

    #ifdef CONSTANT_COLORS
    vec3 albedo     = u_material.albedo;
    float metallic  = u_material.metallic;
    float roughness = u_material.roughness;
    float ao        = u_material.ao;
    vec3 emissive   = u_material.emissive;
    #endif

    #ifdef NORMAL_MAP
    vec3 N = getNormalFromMap();
    #else
    vec3 N = normalize(v_normal);
    #endif
    
    vec3 V = normalize(u_cam_pos - v_position);

    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    // reflectance equation
    vec3 Lo = vec3(0.0);
    for(int i = 0; i < u_num_point_lights; ++i) 
    {
        PointLight l = u_point_lights[i];
        // calculate per-light radiance
        vec3 L = normalize(l.position - v_position);
        vec3 H = normalize(V + L);
        float distance = length(l.position - v_position);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = l.color * attenuation;

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(N, H, roughness);   
        float G   = GeometrySmith(N, V, L, roughness);      
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);
           
        vec3 numerator    = NDF * G * F; 
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
        vec3 specular = numerator / denominator;
        
        // kS is equal to Fresnel
        vec3 kS = F;
        // for energy conservation, the diffuse and specular light can't
        // be above 1.0 (unless the surface emits light); to preserve this
        // relationship the diffuse component (kD) should equal 1.0 - kS.
        vec3 kD = vec3(1.0) - kS;
        // multiply kD by the inverse metalness such that only non-metals 
        // have diffuse lighting, or a linear blend if partly metal (pure metals
        // have no diffuse light).
        kD *= 1.0 - metallic;	  

        // scale light by NdotL
        float NdotL = max(dot(N, L), 0.0);        

        // add to outgoing radiance Lo
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
    }   
    
    // ambient lighting (note that the next IBL tutorial will replace 
    // this ambient lighting with environment lighting).
    vec3 ambient = vec3(0.03) * albedo * ao;
    
    vec3 color = ambient + Lo + emissive;

    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0/2.2)); 

    fragColor = vec4(color, 1.0);
}


float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

#ifdef NORMAL_MAP
vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(normalMap, v_tex_coords).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(v_position);
    vec3 Q2  = dFdy(v_position);
    vec2 st1 = dFdx(v_tex_coords);
    vec2 st2 = dFdy(v_tex_coords);

    vec3 N   = normalize(v_normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}
#endif
