#ifndef __EGE_MOUSE_EVENTS_HPP__
#define __EGE_MOUSE_EVENTS_HPP__

#include <sstream>
#include <glm/glm.hpp>

#include "event.hpp"

namespace ege {

class mouse_moved_event : public ege_event_r {
public:

    inline mouse_moved_event(entt::registry& t_r, const glm::vec2& t_pos, const glm::vec2& t_dpos) : ege_event_r(t_r), m_pos(t_pos), m_dpos(t_dpos) {}

    inline glm::vec2 get_pos() const { return m_pos; }
    inline glm::vec2 get_delta_pos() const { return m_dpos; }

    inline std::string toString() const override {
        std::stringstream ss;
        ss << "mouseMovedEvent: {" << m_pos.x << ", " << m_pos.y << "}, {" << m_dpos.x << ", " << m_dpos.y << "}";
        return ss.str();
    }

    EVENT_CLASS_TYPE(ege::ege_events, mouse_moved)

private:

    glm::vec2 m_pos;
    glm::vec2 m_dpos;
};

class mouse_scrolled_event : public ege_event_r {
public:

    inline mouse_scrolled_event(entt::registry& t_r, const glm::vec2& t_offset) : ege_event_r(t_r), m_offset(t_offset) {}

    inline glm::vec2 get_offset() const { return m_offset; }

    inline std::string toString() const override {
        std::stringstream ss;
        ss << "mouseMovedEvent: {" << m_offset.x << ", " << m_offset.y << "}";
        return ss.str();
    }

    EVENT_CLASS_TYPE(ege::ege_events, mouse_scrolled)

private:

    glm::vec2 m_offset;
};

class mouse_button_event : public ege_event_r {
public:

    inline int get_mouse_button() const { return m_button; }

protected:

    inline mouse_button_event(entt::registry& t_r, const int& t_button) : ege_event_r(t_r), m_button(t_button) {}

    int m_button;
};

class mouse_button_pressed_event : public mouse_button_event {
public:

    inline mouse_button_pressed_event(entt::registry& t_r, const int& t_button) : mouse_button_event(t_r, t_button) {}

    inline std::string toString() const override {
        std::stringstream ss;
        ss << "mouseButtonReleasedEvent: " << m_button;
        return ss.str();
    }

    EVENT_CLASS_TYPE(ege::ege_events, mouse_button_pressed)
};

class mouse_button_released_event : public mouse_button_event {
public:

    inline mouse_button_released_event(entt::registry& t_r, const int& t_button) : mouse_button_event(t_r, t_button) {}

    inline std::string toString() const override {
        std::stringstream ss;
        ss << "mouseButtonReleasedEvent: " << m_button;
        return ss.str();
    }

    EVENT_CLASS_TYPE(ege_events, mouse_button_released)
};

}

#endif
