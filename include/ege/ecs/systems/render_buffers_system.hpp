#ifndef __EGE_RENDER_BUFFERS_SYSTEM_HPP__
#define __EGE_RENDER_BUFFERS_SYSTEM_HPP__

#include <ege/ecs/system.hpp>
#include <ere/api/framebuffer_api.hpp>

namespace ege {

class render_buffers_system : public system {
public:

    virtual bool on_pre_draw(pre_draw_event& event) override;

};

}


#endif // __EGE_RENDER_BUFFERS_SYSTEM_HPP__
