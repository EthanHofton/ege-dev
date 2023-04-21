#ifndef __EGE_RENDER_HPP__
#define __EGE_RENDER_HPP__

#include <ege/ecs/system.hpp>
#include <ere/api/framebuffer_api.hpp>

namespace ege {

class render_system : public system {
public:

    render_system();
    ~render_system() = default;

    void on_draw(ere::ref<ere::framebuffer_api> t_drawbuffer, entt::registry& t_registery) override;

};

}


#endif // __EGE_RENDER_HPP__
