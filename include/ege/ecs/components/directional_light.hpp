#ifndef __EGE_DIRECTIONAL_LIGHT_HPP__
#define __EGE_DIRECTIONAL_LIGHT_HPP__

#include <glm/glm.hpp>
#include <ege/ecs/systems/inspector_system.hpp>

namespace ege {

struct directional_light {
    glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
    float intensity = 1.0f;
};

template<>
inline void inspector_system::component_editor_widget<directional_light>(entt::registry& t_registery, entt::entity t_entity) {
    auto& d = t_registery.get<directional_light>(t_entity);

    ImGui::Text("Color");
    ImGui::DragFloat3("##color", &d.color.x, 1);
    ImGui::Text("Intensity");
    ImGui::DragFloat("##intensity", &d.intensity, 1);
}

}

#endif // __EGE_DIRECTIONAL_LIGHT_HPP__
