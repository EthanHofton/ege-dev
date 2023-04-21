#ifndef __EGE_TRANSFORM_HPP__
#define __EGE_TRANSFORM_HPP__

#include <glm/glm.hpp>
#include <ege/ecs/systems/inspector_system.hpp>

namespace ege {

struct transform {
    glm::vec3 m_position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 m_rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 m_scale = glm::vec3(1.0f, 1.0f, 1.0f);
};

template<>
inline void inspector_system::component_editor_widget<transform>(entt::registry& t_registery, entt::entity t_entity) {
    auto& t = t_registery.get<transform>(t_entity);

    ImGui::Text("Position");
    ImGui::DragFloat3("##position", &t.m_position.x, 0.1f);
    ImGui::Text("Rotation");
    ImGui::DragFloat3("##rotation", &t.m_rotation.x, 0.1f);
    ImGui::Text("Scale");
    ImGui::DragFloat3("##scale", &t.m_scale.x, 0.1f);
}

}


#endif // __EGE_TRANSFORM_HPP__
