#ifndef __EGE_SCENE_SYSTEM_HPP__
#define __EGE_SCENE_SYSTEM_HPP__

#include <ere/core/core.hpp>
#include <ege/ecs/system.hpp>
#include <ere/events/app_events.hpp>
#include <ere/events/key_events.hpp>
#include <ere/events/mouse_events.hpp>

namespace ege {

class scene;
class scene_system : public system {
public:

    bool on_gui_draw(gui_draw_event& event) override;

    inline void select_scene(ere::ref<scene> t_scene) { m_selected_scene = t_scene; }
    void select_scene(const std::string& t_name);
    inline ere::ref<scene> get_selected_scene() const { return m_selected_scene; }

    inline void select_entity(entt::entity t_entity) { m_selected_entity = t_entity; }
    inline entt::entity get_selected_entity() const { return m_selected_entity; }

    ere::ref<scene> add_scene(const std::string& t_name);
    void add_scene(ere::ref<scene> t_scene);

    ere::ref<scene> get_scene(const std::string& t_name);

    void remove_scene(const std::string& t_name);
    void remove_scene(ere::ref<scene> t_scene);


    void on_update(ere::update_event& event);
    void on_imgui_update(ere::imgui_update_event& event);

    void on_key_pressed(ere::key_pressed_event& event);
    void on_key_released(ere::key_released_event& event);
    void on_key_typed(ere::key_typed_event& event);

    void on_mouse_button_pressed(ere::mouse_button_pressed_event& event);
    void on_mouse_button_released(ere::mouse_button_released_event& event);
    void on_mouse_moved(ere::mouse_moved_event& event);
    void on_mouse_scrolled(ere::mouse_scrolled_event& event);

private:

    std::vector<ere::ref<scene>> m_scenes;
    ere::ref<scene> m_selected_scene = nullptr;
    entt::entity m_selected_entity = entt::null;

};

}

#endif // __EGE_SCENE_SYSTEM_HPP__
