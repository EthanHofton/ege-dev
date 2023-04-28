#ifndef __EGE_AREA_LIGHT_HPP__
#define __EGE_AREA_LIGHT_HPP__

#include <ege/ecs/systems/inspector_system.hpp>

namespace ege {

struct area_light {
    glm::vec3 m_color = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 m_normal = glm::vec3(0.0f, 0.0f, 1.0f);
    float m_intensity = 1.0f;
    float m_width = 1.0f;
    float m_height = 1.0f;
};

template<>
inline void inspector_system::component_editor_widget<area_light>(entt::registry& t_registery, entt::entity t_entity) {
    auto& t = t_registery.get<area_light>(t_entity);

    ImGui::Text("Color");
    ImGui::ColorEdit3("##color", &t.m_color.x);
    ImGui::Text("Normal");
    ImGui::DragFloat3("##normal", &t.m_normal.x, 1);
    ImGui::Text("Intensity");
    ImGui::DragFloat("##intensity", &t.m_intensity, 1);
    ImGui::Text("Width");
    ImGui::DragFloat("##width", &t.m_width, 1);
    ImGui::Text("Height");
    ImGui::DragFloat("##height", &t.m_height, 1);
}

}

#endif // __EGE_AREA_LIGHT_HPP__
