#include <ege/ecs/systems/scene_view_system.hpp>
#include <ege/core/logger.hpp>
#include <ere/api/render_api.hpp>
#include <ege/ecs/components/renderer.hpp>
#include <ege/ecs/components/transform.hpp>
#include <ege/ecs/components/mesh.hpp>

#include <imgui.h>

namespace ege {


scene_view_system::scene_view_system() {
    m_drawbuffer = ere::framebuffer_api::create_framebuffer_api({ 1280, 720 });
    m_drawbuffer->add_color_attachment();
    m_drawbuffer->add_depth_stencil_attachment_write_only();


    std::string m_shader_vert = 
        "#version 330 core\n"

        "layout (location = 0) in vec3 aPos;\n"
        // "layout (location = 1) in vec3 aNormal;\n"
        // "layout (location = 2) in vec3 aTangent;\n"
        // "layout (location = 3) in vec3 aBitangent;\n"
        // "layout (location = 4) in vec2 aUV;\n"

        // "out vec3 Normal;\n"
        // "out vec3 FragPos;\n"

        "uniform mat4 u_model;\n"
        "uniform mat4 u_view;\n"
        "uniform mat4 u_projection;\n"

        "void main()\n"
        "{\n"
        "   gl_Position = u_projection * u_view * u_model * vec4(aPos, 1.0);\n"
        // "   Normal = mat3(transpose(inverse(model))) * aNormal;\n"
        // "   FragPos = vec3(model * vec4(aPos, 1.0));\n"
        "}\0";

    std::string m_shader_frag =
        "#version 330 core\n"

        "out vec4 FragColor;\n"

        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\n\0";

    m_shader = ere::shader_api::create_shader_api(m_shader_vert, m_shader_frag);
    m_camera = ere::createRef<camera_test>();
}

bool scene_view_system::on_pre_draw(pre_draw_event& event) {
    // bind framebuffer
    m_drawbuffer->bind();

    // clear colors
    ere::render_api::clear_color({ 0.1f, 0.1f, 0.1f, 1.0f });
    ere::render_api::clear_buffers();
    
    return false;
}

bool scene_view_system::on_draw(draw_event& event) {

    auto view = event.get_registry().view<renderer>();
    for (auto& e : view) {
        auto& ren = view.get<renderer>(e);

        m_shader->bind();

        if (event.get_registry().any_of<transform>(e)) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, event.get_registry().get<transform>(e).m_position);
            model = glm::rotate(model, glm::radians(event.get_registry().get<transform>(e).m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, glm::radians(event.get_registry().get<transform>(e).m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, glm::radians(event.get_registry().get<transform>(e).m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::scale(model, event.get_registry().get<transform>(e).m_scale);
            m_shader->set_uniform_mat4f("u_model", model);
        }

        ere::render_api::set_camera(m_camera);

        if (ren.m_index_buffer_api) {
            ere::render_api::draw_indexed(ren.m_vertex_array_api, m_shader);
        } else {
            if (event.get_registry().any_of<mesh>(e)) {
                ere::render_api::draw_arrays(ren.m_vertex_array_api, m_shader, event.get_registry().get<mesh>(e).m_positions.size());
            }
        }

    }

    return false;
}

bool scene_view_system::on_post_draw(post_draw_event& event) {

    // unbind
    m_drawbuffer->unbind();

    return false;
}

bool scene_view_system::on_gui_draw(gui_draw_event& event) {
    ImGui::Begin("Scene View");

    ImVec2 imageSize = ImGui::GetContentRegionAvail();
    if (m_drawbuffer->get_width() != imageSize.x || m_drawbuffer->get_height() != imageSize.y) {
        m_drawbuffer->resize(imageSize.x, imageSize.y);
        m_camera->set_aspect_ratio(imageSize.x / imageSize.y);
    }

    ImGui::Image((void*)(size_t)m_drawbuffer->get_color_attachemt()->get_texture_id(), imageSize);

    ImGui::End();
    
    return false;
}

}
