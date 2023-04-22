#include <ege/ecs/systems/scene_view_system.hpp>
#include <ege/core/logger.hpp>
#include <ere/api/render_api.hpp>

#include <imgui.h>

namespace ege {

scene_view_system::scene_view_system() {
    m_drawbuffer = ere::framebuffer_api::create_framebuffer_api({ 1280, 720 });
    m_drawbuffer->add_color_attachment();
    m_drawbuffer->add_depth_stencil_attachment_write_only();
}

bool scene_view_system::on_pre_draw(pre_draw_event& event) {
    return false;
}

bool scene_view_system::on_draw(draw_event& event) {
    
    // bind framebuffer
    m_drawbuffer->bind();

    // clear colors
    ere::render_api::clear_color({ 0.1f, 0.1f, 0.1f, 1.0f });
    ere::render_api::clear_buffers();

    // draw here

    // unbind
    m_drawbuffer->unbind();

    return false;
}

bool scene_view_system::on_post_draw(post_draw_event& event) {
    return false;
}

bool scene_view_system::on_gui_draw(gui_draw_event& event) {
    ImGui::Begin("Scene View");

    ImVec2 imageSize = ImGui::GetContentRegionAvail();
    if (m_drawbuffer->get_width() != imageSize.x || m_drawbuffer->get_height() != imageSize.y) {
        m_drawbuffer->resize(imageSize.x, imageSize.y);
    }

    ImGui::Image((void*)(size_t)m_drawbuffer->get_color_attachemt()->get_texture_id(), imageSize);

    ImGui::End();
    
    return false;
}

}
