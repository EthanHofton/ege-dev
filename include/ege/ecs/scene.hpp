#ifndef __EGE_SCENE_HPP__
#define __EGE_SCENE_HPP__

#include <ere/core/layer.hpp>
#include <ere/core/core.hpp>
#include <entt/entt.hpp>
#include <memory>

#include "system.hpp"
#include <ege/ecs/systems/inspector.hpp>
#include <ege/ecs/components/UUID.hpp>
#include <ege/ecs/components/tag.hpp>

namespace ege {

class scene : public std::enable_shared_from_this<scene> {
public:

    scene();

    void on_update(ere::update_event& event);
    void on_imgui_update(ere::imgui_update_event& event);

    void on_attach(ere::attach_event& event);
    void on_detach(ere::detach_event& event);

    void on_key_pressed(ere::key_pressed_event& event);
    void on_key_released(ere::key_released_event& event);
    void on_key_typed(ere::key_typed_event& event);

    void on_mouse_button_pressed(ere::mouse_button_pressed_event& event);
    void on_mouse_button_released(ere::mouse_button_released_event& event);
    void on_mouse_moved(ere::mouse_moved_event& event);
    void on_mouse_scrolled(ere::mouse_scrolled_event& event);

    void on_window_resized(ere::window_resized_event& event);
    void on_window_moved(ere::window_moved_event& event);
    void on_window_minimized(ere::window_minimized_event& event);
    void on_window_maximized(ere::window_maximized_event& event);
    void on_window_focus(ere::window_focus_event& event);
    void on_window_closed(ere::window_closed_event& event);

    void on_hierarchy_draw(ere::ref<scene>& t_active_scene);
    void on_inspector_draw();

    void on_scene_viewport_draw();
    void simulate();

    entt::entity create_entity();
    entt::entity create_entity(const std::string& name);

    inline entt::registry& get_registry() { return m_registry; }

    inline std::string get_scene_name() { return m_scene_name; }
    inline void set_scene_name(std::string name) { m_scene_name = name; }

    inline void add_system(std::shared_ptr<system> system) { m_systems.push_back(system); }
    inline void remove_system(std::shared_ptr<system> system) { m_systems.erase(std::remove(m_systems.begin(), m_systems.end(), system), m_systems.end()); }
    inline std::vector<std::shared_ptr<system>> get_systems() { return m_systems; }

    inline entt::entity get_selected_entity() { return m_selected_entity; }

private:

    entt::registry m_registry;
    entt::entity m_selected_entity = entt::null;
    std::vector<std::shared_ptr<system>> m_systems;

    std::string m_scene_name;
    ere::ref<inspector_system> m_debug_system;

};

}

#endif // __EGE_SCENE_HPP__
