#ifndef __EGE_SCENE_VIEW_SYSTEM_HPP__
#define __EGE_SCENE_VIEW_SYSTEM_HPP__

#include <ege/ecs/system.hpp>
#include <ere/api/framebuffer_api.hpp>

namespace ege {

class scene_view_system : public system {
public:

    scene_view_system();

    virtual bool on_pre_draw(pre_draw_event& event) override;
    virtual bool on_draw(draw_event& event) override;
    virtual bool on_post_draw(post_draw_event& event) override;

    virtual bool on_gui_draw(gui_draw_event& event) override;

private:

    ere::ref<ere::framebuffer_api> m_drawbuffer;

};

}

#endif // __EGE_SCENE_VIEW_SYSTEM_HPP__
