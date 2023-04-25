#ifndef __EGE_MATERIAL_HPP__
#define __EGE_MATERIAL_HPP__

#include <ere/api/texture_api.hpp>
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

        static ere::ref<ere::shader_api> constant_shader = ere::shader_api::create_shader_api_from_file("res/shaders/material/constant_shader/vert.glsl", "res/shaders/material/constant_shader/frag.glsl");

        // static ere::ref<shader_api> texture_normal_shader = ere::shader_api::create_shader_api_from_file();
        // static ere::ref<shader_api> constant_shader = ere::shader_api::create_shader_api_from_file();

        // m.m_texture_shader = texture_shader;
        m.m_texture_shader = nullptr;
        // m.m_texture_normal_shader = texture_normal_shader;
        m.m_texture_normal_shader = nullptr;
        m.m_constant_shader = constant_shader;
        // m.m_constant_shader = nullptr;

        m.m_active_shader = m.m_constant_shader;
        
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
}

template<>
inline void inspector_system::component_add<material>(entt::registry& t_registery, entt::entity t_entity) {
    t_registery.emplace<material>(t_entity, material_generator::generate_default_material());
}

}

#endif
