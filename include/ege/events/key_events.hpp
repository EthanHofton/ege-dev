#ifndef __EGE_KEY_EVENTS_HPP__
#define __EGE_KEY_EVENTS_HPP__

#include <sstream>
#include "event.hpp"

namespace ege {

class key_event : public ege_event_r {
public:

    inline int get_key_code() const { return m_keycode; }
    
protected:

    inline key_event(entt::registry& t_r, const int& t_keycode) : ege_event_r(t_r), m_keycode(t_keycode) {}

    int m_keycode;
};

class key_pressed_event : public key_event {
public:

    inline key_pressed_event(entt::registry& t_e, int t_keycode, int t_repeteCount) : key_event(t_e, t_keycode), m_repeteCount(t_repeteCount) {}

    inline int get_repete_count() const { return m_repeteCount; }

    inline std::string toString() const override
    {
        std::stringstream ss;
        ss << "KeyPressedEvent: " << m_keycode << " (" << m_repeteCount << " repeats)";
        return ss.str();
    }

    EVENT_CLASS_TYPE(ege::ege_events, key_pressed)

private:

    int m_repeteCount;
};

class key_released_event : public key_event
{
public:

    inline key_released_event(entt::registry& t_e, int t_keycode) : key_event(t_e, t_keycode) {}

    inline std::string toString() const override
    {
        std::stringstream ss;
        ss << "KeyReleasedEvent: " << m_keycode;
        return ss.str();
    }

    EVENT_CLASS_TYPE(ege::ege_events, key_released);
};

class key_typed_event : public key_event
{
public:

    inline key_typed_event(entt::registry& t_r, int t_keycode) : key_event(t_r, t_keycode) {}

    inline std::string toString() const override
    {
        std::stringstream ss;
        ss << "KeyTypedEvent: " << m_keycode;
        return ss.str();
    }

    EVENT_CLASS_TYPE(ege::ege_events, key_typed);
};

}

#endif
