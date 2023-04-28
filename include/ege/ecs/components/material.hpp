#ifndef __EGE_MATERIAL_HPP__
#define __EGE_MATERIAL_HPP__

#include <ere/api/texture2d_api.hpp>
#include <ere/api/shader_api.hpp>
#include <ege/ecs/systems/inspector_system.hpp>

namespace ege {

struct material {
    // constants
    glm::vec3 m_albedo_color;
    float m_metallic_value;
    float m_roughness_value;
    float m_ao_value;
    glm::vec3 m_emission_color;

    // textures
    ere::ref<ere::texture_api> m_albedo;
    ere::ref<ere::texture_api> m_normal;
    ere::ref<ere::texture_api> m_metallic;
    ere::ref<ere::texture_api> m_roughness;
    ere::ref<ere::texture_api> m_ao;
    ere::ref<ere::texture_api> m_emission;

    // shader
    ere::ref<ere::shader_api> m_texture_shader;
    ere::ref<ere::shader_api> m_texture_normal_shader;
    ere::ref<ere::shader_api> m_constant_shader;

    // active shader
    ere::ref<ere::shader_api> m_active_shader;
};

struct material_generator {
    static material generate_default_material() {
        material m;

        m.m_albedo_color = glm::vec3(1.0f, 1.0f, 1.0f);
        m.m_metallic_value = 0.0f;
        m.m_roughness_value = 0.5f;
        m.m_ao_value = 1.0f;
        m.m_emission_color = glm::vec3(0.0f, 0.0f, 0.0f);

        m.m_albedo = nullptr;
        m.m_normal = nullptr;
        m.m_metallic = nullptr;
        m.m_roughness = nullptr;
        m.m_ao = nullptr;
        m.m_emission = nullptr;

        ere::ref<ere::shader_api> constant_shader = ere::shader_api::create_shader_api_from_file("res/shaders/material/constant_shader/vert.glsl", "res/shaders/material/constant_shader/frag.glsl");
        ere::ref<ere::shader_api> texture_shader = ere::shader_api::create_shader_api_from_file("res/shaders/material/texture_shader/vert.glsl", "res/shaders/material/texture_shader/frag.glsl");
        ere::ref<ere::shader_api> texture_normal_shader = ere::shader_api::create_shader_api_from_file("res/shaders/material/texture_normal_shader/vert.glsl", "res/shaders/material/texture_normal_shader/frag.glsl");

        m.m_texture_shader = texture_shader;
        m.m_texture_normal_shader = texture_normal_shader;
        m.m_constant_shader = constant_shader;

        m.m_active_shader = m.m_constant_shader;
        
        return m;
    }

    static material generate_rustic_sphere_material() {
        material m;
        
        ere::ref<ere::shader_api> constant_shader = ere::shader_api::create_shader_api_from_file("res/shaders/material/constant_shader/vert.glsl", "res/shaders/material/constant_shader/frag.glsl");
        ere::ref<ere::shader_api> texture_shader = ere::shader_api::create_shader_api_from_file("res/shaders/material/texture_shader/vert.glsl", "res/shaders/material/texture_shader/frag.glsl");
        ere::ref<ere::shader_api> texture_normal_shader = ere::shader_api::create_shader_api_from_file("res/shaders/material/texture_normal_shader/vert.glsl", "res/shaders/material/texture_normal_shader/frag.glsl");

        m.m_albedo = ere::texture2d_api::create_texture2d_api("res/materials/rustic_metal/albedo.png");
        m.m_normal = ere::texture2d_api::create_texture2d_api("res/materials/rustic_metal/normal.png");
        m.m_metallic = ere::texture2d_api::create_texture2d_api("res/materials/rustic_metal/metallic.png");
        m.m_roughness = ere::texture2d_api::create_texture2d_api("res/materials/rustic_metal/roughness.png");
        m.m_ao = ere::texture2d_api::create_texture2d_api("res/textures/white.png");
        m.m_emission = ere::texture2d_api::create_texture2d_api("res/textures/black.png");

        m.m_texture_shader = texture_shader;
        m.m_texture_normal_shader = texture_normal_shader;
        m.m_constant_shader = constant_shader;

        m.m_active_shader = m.m_texture_normal_shader;

        return m;
    }

    static material generate_bick_material() {
        material m;

        ere::ref<ere::shader_api> constant_shader = ere::shader_api::create_shader_api_from_file("res/shaders/material/constant_shader/vert.glsl", "res/shaders/material/constant_shader/frag.glsl");
        ere::ref<ere::shader_api> texture_shader = ere::shader_api::create_shader_api_from_file("res/shaders/material/texture_shader/vert.glsl", "res/shaders/material/texture_shader/frag.glsl");
        ere::ref<ere::shader_api> texture_normal_shader = ere::shader_api::create_shader_api_from_file("res/shaders/material/texture_normal_shader/vert.glsl", "res/shaders/material/texture_normal_shader/frag.glsl");

        m.m_albedo = ere::texture2d_api::create_texture2d_api("res/materials/brickwall/brickwall.jpg");
        m.m_normal = ere::texture2d_api::create_texture2d_api("res/materials/brickwall/brickwall_normal.jpg");
        m.m_metallic = ere::texture2d_api::create_texture2d_api("res/textures/black.png");
        m.m_roughness = ere::texture2d_api::create_texture2d_api("res/textures/black.png");
        m.m_ao = ere::texture2d_api::create_texture2d_api("res/textures/white.png");
        m.m_emission = ere::texture2d_api::create_texture2d_api("res/textures/black.png");

        m.m_texture_shader = texture_shader;
        m.m_texture_normal_shader = texture_normal_shader;
        m.m_constant_shader = constant_shader;

        m.m_active_shader = m.m_texture_normal_shader;

        return m;


    }
};

template<>
inline void inspector_system::component_editor_widget<material>(entt::registry& t_registery, entt::entity t_entity) {
    auto& m = t_registery.get<material>(t_entity);

    ImGui::Text("Albedo Color");
    ImGui::DragFloat3("##albedo_color", &m.m_albedo_color.x, 0.01f);
    ImGui::Text("Metallic Value");
    ImGui::DragFloat("##metallic_value", &m.m_metallic_value, 0.01f);
    ImGui::Text("Roughness Value");
    ImGui::DragFloat("##roughness_value", &m.m_roughness_value, 0.01f);
    ImGui::Text("AO Value");
    ImGui::DragFloat("##ao_value", &m.m_ao_value, 0.01f);
    ImGui::Text("Emission Color");
    ImGui::DragFloat3("##emission_color", &m.m_emission_color.x, 0.01f);

    if (ImGui::Button("Default Material")) {
        m = material_generator::generate_default_material();
    }

    if (ImGui::Button("Rustic Sphere Material")) {
        m = material_generator::generate_rustic_sphere_material();
    }

    if (ImGui::Button("Brickwall Material")) {
        m = material_generator::generate_bick_material();
    }
}

template<>
inline void inspector_system::component_add<material>(entt::registry& t_registery, entt::entity t_entity) {
    t_registery.emplace<material>(t_entity, material_generator::generate_default_material());
}

}

#endif
