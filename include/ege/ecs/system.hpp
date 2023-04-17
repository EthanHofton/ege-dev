#ifndef __EGE_SYSTEM_HPP__
#define __EGE_SYSTEM_HPP__

#include <ere/core/layer.hpp>
#include <entt/entt.hpp>

namespace ege {

class system {
public:

    virtual void on_update(ere::update_event& event, entt::registry& t_registery) {}
    virtual void on_imgui_update(ere::imgui_update_event& event, entt::registry& t_registery) {}

    virtual void on_attach(ere::attach_event& event, entt::registry& t_registery) {}
    virtual void on_detach(ere::detach_event& event, entt::registry& t_registery) {}

    virtual void on_key_pressed(ere::key_pressed_event& event, entt::registry& t_registery) {}
    virtual void on_key_released(ere::key_released_event& event, entt::registry& t_registery) {}
    virtual void on_key_typed(ere::key_typed_event& event, entt::registry& t_registery) {}

    virtual void on_mouse_button_pressed(ere::mouse_button_pressed_event& event, entt::registry& t_registery) {}
    virtual void on_mouse_button_released(ere::mouse_button_released_event& event, entt::registry& t_registery) {}
    virtual void on_mouse_moved(ere::mouse_moved_event& event, entt::registry& t_registery) {}
    virtual void on_mouse_scrolled(ere::mouse_scrolled_event& event, entt::registry& t_registery) {}

    virtual void on_window_resized(ere::window_resized_event& event, entt::registry& t_registery) {}
    virtual void on_window_moved(ere::window_moved_event& event, entt::registry& t_registery) {}
    virtual void on_window_minimized(ere::window_minimized_event& event, entt::registry& t_registery) {}
    virtual void on_window_maximized(ere::window_maximized_event& event, entt::registry& t_registery) {}
    virtual void on_window_focus(ere::window_focus_event& event, entt::registry& t_registery) {}
    virtual void on_window_closed(ere::window_closed_event& event, entt::registry& t_registery) {}

    virtual void on_inspector_draw(entt::registry& t_registery, entt::entity t_entity) {}

};

}

#endif // __EGE_SYSTEM_HPP__
