#include <ege/ecs/systems/inspector_system.hpp>
#include <ege/ecs/scene.hpp>
#include <misc/cpp/imgui_stdlib.h>

namespace ege {

bool inspector_system::on_gui_draw(gui_draw_event& t_r) {
    ImGui::Begin("Inspector");

    entt::entity activate_entity = system_manager::get<scene_system>()->get_selected_entity();

    if (activate_entity == entt::null || t_r.get_registry().valid(activate_entity) == false) {
        ImGui::Text("Scene: %s", system_manager::get<scene_system>()->get_selected_scene()->get_scene_name().c_str());

        ImGui::InputText("Scene name", &system_manager::get<scene_system>()->get_selected_scene()->get_scene_name());

        ImGui::End();
        return false;
    }


    // Draw all registered components
    for (auto& component : m_registered_components) {
        // Check if component exists
        if (component.exists(t_r.get_registry(), activate_entity) == false) continue;

        // Component name with dropdown
        if (ImGui::CollapsingHeader(component.name.c_str())) {
            ImGui::Spacing();
            component.draw(t_r.get_registry(), activate_entity);
            ImGui::Spacing();

            if (component.removable) {
                // Remove component button
                ImGui::PushID(component.name.c_str());
                if (ImGui::Button("Remove component")) {
                    component.remove(t_r.get_registry(), activate_entity);
                }
                ImGui::PopID();
            }
            ImGui::Spacing();
        }
    }

    ImGui::Spacing();
    // Add new registered component
    if (ImGui::BeginCombo("Add component", "Component")) {
        for (auto& component : m_registered_components) {
            if (component.exists(t_r.get_registry(), activate_entity)) continue;
            if (component.addable == false) continue;

            if (ImGui::Selectable(component.name.c_str())) {
                component.add(t_r.get_registry(), activate_entity);
            }
        }
        ImGui::EndCombo();
    }

    ImGui::End();

    return false;
}

}
