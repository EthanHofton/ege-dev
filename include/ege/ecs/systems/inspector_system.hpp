#ifndef __EGE_DEBUG_HPP__
#define __EGE_DEBUG_HPP__

#include <ege/ecs/system.hpp>
#include "scene_system.hpp"
#include <ege/ecs/system_manager.hpp>
#include <imgui.h>

namespace ege {

class inspector_system : public system {
public:

    template<class Component>
    void component_editor_widget(entt::registry& t_registery, entt::entity t_entity) {}

    template<class Component>
    void component_add(entt::registry& t_registery, entt::entity t_entity) { t_registery.emplace<Component>(t_entity); }

    template<class Component>
    void component_remove(entt::registry& t_registery, entt::entity t_entity) { t_registery.remove<Component>(t_entity); }

    template<class Component>
    bool component_exists(entt::registry& t_registery, entt::entity t_entity) { return t_registery.any_of<Component>(t_entity); }

    template<class Component>
    void register_component(const std::string& t_name, bool t_removable = true, bool t_addable = true) {
        if (std::find(m_registered_components.begin(), m_registered_components.end(), t_name) != m_registered_components.end()) {
            return;
        }

        m_registered_components.push_back({
            .name = t_name, 
            .draw = std::bind(&inspector_system::component_editor_widget<Component>, this, std::placeholders::_1, std::placeholders::_2),
            .add = std::bind(&inspector_system::component_add<Component>, this, std::placeholders::_1, std::placeholders::_2),
            .remove = std::bind(&inspector_system::component_remove<Component>, this, std::placeholders::_1, std::placeholders::_2),
            .exists = std::bind(&inspector_system::component_exists<Component>, this, std::placeholders::_1, std::placeholders::_2),
            .removable = t_removable,
            .addable = t_addable
        });
    }

    bool on_gui_draw(gui_draw_event& t_r) override;

private:

    struct component_info {
        std::string name;
        std::function<void(entt::registry&, entt::entity)> draw;
        std::function<void(entt::registry&, entt::entity)> add;
        std::function<void(entt::registry&, entt::entity)> remove;
        std::function<bool(entt::registry&, entt::entity)> exists;

        bool removable = true;
        bool addable = true;

        bool operator==(const component_info& t_l) const {
            return name == t_l.name;
        }

        bool operator==(const std::string& t_l) const {
            return name == t_l;
        }
    };

    std::vector<component_info> m_registered_components;

};

}

#endif // __EGE_DEBUG_HPP__
