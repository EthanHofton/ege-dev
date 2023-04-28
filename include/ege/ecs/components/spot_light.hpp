#ifndef __EGE_SPOT_LIGHT_HPP__
#define __EGE_SPOT_LIGHT_HPP__

#include <ege/ecs/systems/inspector_system.hpp>

namespace ege {

struct spot_light {
    glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
    float intensity = 1.0f;
    float cutoff = 12.5f;
    float outer_cutoff = 17.5f;
    float constant = 1.0f;
    float linear = 0.09f;
    float quadratic = 0.032f;
};

template<>
inline void inspector_system::component_editor_widget<spot_light>(entt::registry& t_registery, entt::entity t_entity) {
    auto& light = t_registery.get<spot_light>(t_entity);

    ImGui::Text("Color");
    ImGui::ColorEdit3("##color", &light.color.x);
    ImGui::Text("Cutoff");
    ImGui::DragFloat("##cutoff", &light.cutoff, 0.1f, 0.0f, 180.0f);
    ImGui::Text("Outer cutoff");
    ImGui::DragFloat("##outer_cutoff", &light.outer_cutoff, 0.1f, 0.0f, 180.0f);
    ImGui::Text("Constant");
    ImGui::DragFloat("##constant", &light.constant, 0.1f, 0.0f, 1.0f);
    ImGui::Text("Linear");
    ImGui::DragFloat("##linear", &light.linear, 0.1f, 0.0f, 1.0f);
    ImGui::Text("Quadratic");
    ImGui::DragFloat("##quadratic", &light.quadratic, 0.1f, 0.0f, 1.0f);
}

}

#endif // __EGE_SPOT_LIGHT_HPP__
