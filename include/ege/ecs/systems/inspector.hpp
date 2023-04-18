#ifndef __EGE_DEBUG_HPP__
#define __EGE_DEBUG_HPP__

#include <ege/ecs/system.hpp>
#include <imgui.h>

namespace ege {

class inspector_system : public system {
public:

    template<class Component>
    void component_editor_widget(entt::registry& t_registery, entt::entity t_entity) {}

    template<class Component>
    void register_component(const std::string& t_name) {
        m_registered_components.push_back({t_name, std::bind(&inspector_system::component_editor_widget<Component>, this, std::placeholders::_1, std::placeholders::_2)});
    }

    inline void on_inspector_draw(entt::registry& t_registery, entt::entity t_entity) override {
        if (t_entity == entt::null) return;
        if (t_registery.valid(t_entity) == false) return;
        for (auto& component : m_registered_components) {
            // Component name with dropdown
            if (ImGui::CollapsingHeader(component.name.c_str())) {
                ImGui::Indent();
                component.draw(t_registery, t_entity);
                ImGui::Unindent();
            }
        }
    }

private:

    struct component_info {
        std::string name;
        std::function<void(entt::registry&, entt::entity)> draw;
    };

    std::vector<component_info> m_registered_components;


};

}

#endif // __EGE_DEBUG_HPP__
