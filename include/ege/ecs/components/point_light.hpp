#ifndef __EGE_POINT_LIGHT_HPP__
#define __EGE_POINT_LIGHT_HPP__

#include <ege/ecs/systems/inspector_system.hpp>

namespace ege {

struct point_light {
    glm::vec3 m_color = glm::vec3(1.0f, 1.0f, 1.0f);
    float m_intensity = 1.0f;
};

template<>
inline void inspector_system::component_editor_widget<point_light>(entt::registry& t_registery, entt::entity t_entity) {
    auto& t = t_registery.get<point_light>(t_entity);

    ImGui::Text("Color");
    ImGui::DragFloat3("##color", &t.m_color.x);
    ImGui::Text("Intensity");
    ImGui::DragFloat("##intensity", &t.m_intensity, 0.1f, 0.0f, 100.0f);
}

}

#endif // __EGE_POINT_LIGHT_HPP__
