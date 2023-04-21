#ifndef __EGE_SYSTEM_HPP__
#define __EGE_SYSTEM_HPP__

#include "i_system.hpp"
#include <ege/events/app_events.hpp>
#include <ege/events/key_events.hpp>
#include <ege/events/mouse_events.hpp>

namespace ege {

class system : public i_system {
public:

    virtual ~system() = default;

    virtual bool on_pre_draw(pre_draw_event& event) { return false; }
    virtual bool on_draw(draw_event& event) { return false; }
    virtual bool on_gui_draw(gui_draw_event& event) { return false; }
    virtual bool on_post_draw(post_draw_event& event) { return false; }
    virtual bool on_update(update_event& event) { return false; }
    virtual bool on_attach(attach_event& event) { return false; }
    virtual bool on_detach(detach_event& event) { return false; }

    virtual bool on_key_pressed(key_pressed_event& event) { return false; }
    virtual bool on_key_released(key_released_event& event) { return false; }
    virtual bool on_key_typed(key_typed_event& event) { return false; }

    virtual bool on_mouse_button_pressed(mouse_button_pressed_event& event) { return false; }
    virtual bool on_mouse_button_released(mouse_button_released_event& event) { return false; }
    virtual bool on_mouse_moved(mouse_moved_event& event) { return false; }
    virtual bool on_mouse_scrolled(mouse_scrolled_event& event) { return false; }

    virtual void on_event(ege_event& t_e) override final;
};

}

#endif // __EGE_SYSTEM_HPP__
