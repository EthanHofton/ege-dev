#ifndef __EGE_EVENT_HPP__
#define __EGE_EVENT_HPP__

#include <util/event.hpp>
#include <entt/entt.hpp>

namespace ege {

enum class ege_events {
    pre_draw, draw, post_draw, gui_draw, update, attach, detach,
    key_pressed, key_released, key_typed,
    mouse_button_pressed, mouse_button_released, mouse_moved, mouse_scrolled
};

struct ege_event : public util::event<ege_events> {};

class ege_event_r : public ege_event {
public:
    ege_event_r(entt::registry& t_registry) : m_registry(t_registry) {}

    entt::registry& get_registry() { return m_registry; }

protected:

    entt::registry& m_registry;
};

}

#endif
