#ifndef __EGE_SCENE_VIEW_SYSTEM_HPP__
#define __EGE_SCENE_VIEW_SYSTEM_HPP__

#include <ege/ecs/system.hpp>
#include <ere/api/framebuffer_api.hpp>
#include <ere/api/shader_api.hpp>
#include <ere/api/camera_api.hpp>
#include <ere/api/vertex_array_api.hpp>

namespace ege {

class scene_view_system : public system {
public:

    scene_view_system();

    virtual bool on_pre_draw(pre_draw_event& event) override;
    virtual bool on_draw(draw_event& event) override;
    virtual bool on_post_draw(post_draw_event& event) override;

    virtual bool on_update(update_event& event) override;

    virtual bool on_gui_draw(gui_draw_event& event) override;

private:

    void create_grid();

    ere::ref<ere::framebuffer_api> m_drawbuffer;
    ere::ref<ere::shader_api> m_shader;
    ere::ref<ere::shader_api> m_outline_shader;

    struct grid {
        ere::ref<ere::vertex_array_api> vao;
        ere::ref<ere::vertex_buffer_api> vbo_pos;
        std::vector<glm::vec3> vertices;
        ere::ref<ere::vertex_buffer_api> vbo_uv;
        std::vector<glm::vec2> uvs;
        ere::ref<ere::index_buffer_api> ibo;
        std::vector<uint32_t> indices;
        ere::ref<ere::texture2d_api> tex;
        ere::ref<ere::shader_api> shader;

        glm::mat4 transform;
    } m_grid;
};

}

#endif // __EGE_SCENE_VIEW_SYSTEM_HPP__
