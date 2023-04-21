#ifndef __EGE_SCENE_HPP__
#define __EGE_SCENE_HPP__

#include <ere/core/layer.hpp>
#include <ere/core/core.hpp>
#include <entt/entt.hpp>
#include <memory>

#include "i_system.hpp"
#include "system_manager.hpp"
#include <ege/ecs/systems/inspector_system.hpp>
#include <ege/ecs/components/UUID.hpp>
#include <ege/ecs/components/tag.hpp>

namespace ege {

class scene : public std::enable_shared_from_this<scene> {
public:

    scene();

    entt::entity create_entity();
    entt::entity create_entity(const std::string& name);

    void on_select();
    void on_deslect();

    void on_update(ere::update_event& event);
    void on_imgui_update(ere::imgui_update_event& event);

    void on_draw();
    void on_pre_draw();
    void on_post_draw();

    void on_key_pressed(ere::key_pressed_event& event);
    void on_key_released(ere::key_released_event& event);
    void on_key_typed(ere::key_typed_event& event);

    void on_mouse_button_pressed(ere::mouse_button_pressed_event& event);
    void on_mouse_button_released(ere::mouse_button_released_event& event);
    void on_mouse_moved(ere::mouse_moved_event& event);
    void on_mouse_scrolled(ere::mouse_scrolled_event& event);


    inline entt::registry& get_registry() { return m_registry; }

    inline std::string& get_scene_name() { return m_scene_name; }
    inline void set_scene_name(std::string name) { m_scene_name = name; }

private:

    entt::registry m_registry;
    std::string m_scene_name;
};

}

#endif // __EGE_SCENE_HPP__
