#ifndef __EGE_MAIN_LAYER_HPP__
#define __EGE_MAIN_LAYER_HPP__

#include <ege/core/logger.hpp>
#include <ere/core/layer.hpp>
#include <imgui.h>

namespace ege {

class main_layer : public ere::layer {
public:

    bool on_attach(ere::attach_event& e) override;
    bool on_detach(ere::detach_event& e) override;
    bool on_update(ere::update_event& e) override;
    bool on_imgui_update(ere::imgui_update_event& e) override;
};

}

#endif
