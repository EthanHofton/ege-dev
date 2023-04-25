#ifndef __EGE_SCENE_CAMERA_SYSTEM_HPP__
#define __EGE_SCENE_CAMERA_SYSTEM_HPP__

#include <ege/ecs/system.hpp>
#include <ege/ecs/components/scene_camera.hpp>

namespace ege {
class scene_camera_system : public system {
public:

    virtual bool on_update(update_event& event) override;
    virtual bool on_mouse_scrolled(mouse_scrolled_event& event) override;
    virtual bool on_mouse_moved(mouse_moved_event& event) override;

    static entt::entity get_camera_entity(entt::registry& t_registery);
    static scene_camera& get_camera(entt::registry& t_registery);

    void set_focused(bool t_focused) { m_focused = t_focused; }
    bool get_focused() { return m_focused; }

private:

    void update_camera(entt::registry& t_registery, entt::entity &camera_entity, scene_camera& t_camera);
    static entt::entity create_camera(entt::registry& t_registery);

    bool m_focused = false;

};

}

#endif // __EGE_SCENE_CAMERA_SYSTEM_HPP__
