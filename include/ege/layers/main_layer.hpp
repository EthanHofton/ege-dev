#ifndef __EGE_MAIN_LAYER_HPP__
#define __EGE_MAIN_LAYER_HPP__

#include <ege/core/logger.hpp>
#include <ere/core/layer.hpp>
#include <imgui.h>
#include <ege/ecs/scene.hpp>
#include <ere/core/core.hpp>

namespace ege {

class main_layer : public ere::layer {
public:

    bool on_attach(ere::attach_event& e) override;
    bool on_detach(ere::detach_event& e) override;

    bool on_update(ere::update_event& e) override;
    bool on_imgui_update(ere::imgui_update_event& e) override;

    bool on_key_pressed(ere::key_pressed_event& e) override;
    bool on_key_released(ere::key_released_event& e) override;
    bool on_key_typed(ere::key_typed_event& e) override;

    bool on_mouse_button_pressed(ere::mouse_button_pressed_event& e) override;
    bool on_mouse_button_released(ere::mouse_button_released_event& e) override;
    bool on_mouse_moved(ere::mouse_moved_event& e) override;
    bool on_mouse_scrolled(ere::mouse_scrolled_event& e) override;

private:

    void setup_dockspace();
    void draw_menu_bar();
    void draw_project_explorer();
};

}

#endif
