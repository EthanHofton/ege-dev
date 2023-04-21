#ifndef __EGE_APP_EVENTS_HPP__
#define __EGE_APP_EVENTS_HPP__

#include "event.hpp"

namespace ege {

#define EMPTY_EVENT(name) class name##_event : public ege_event { public: EVENT_CLASS_TYPE(ege::ege_events, name); };

#define EMPTY_EVENT_R(name) class name##_event : public ege_event_r { public: name##_event(entt::registry& t_r) : ege_event_r(t_r) {} \
    EVENT_CLASS_TYPE(ege::ege_events, name); };


EMPTY_EVENT(attach);
EMPTY_EVENT(detach);
EMPTY_EVENT_R(draw);
EMPTY_EVENT_R(update);
EMPTY_EVENT_R(pre_draw);
EMPTY_EVENT_R(post_draw);
EMPTY_EVENT_R(gui_draw);

}

#endif
