#include <ege/ecs/systems/scene_view_system.hpp>
#include <ege/core/logger.hpp>
#include <ere/api/render_api.hpp>
#include <ege/ecs/components/renderer.hpp>
#include <ege/ecs/components/transform.hpp>
#include <ege/ecs/components/mesh.hpp>

#include <imgui.h>

namespace ege {


scene_view_system::scene_view_system() {
    m_drawbuffer = ere::framebuffer_api::create_framebuffer_api({ 1675, 820 });
    m_drawbuffer->add_color_attachment();
    m_drawbuffer->add_depth_stencil_attachment_write_only();

    m_drawbuffer->bind();
    ere::render_api::set_viewport({1675, 820});
    m_drawbuffer->unbind();


    std::string m_shader_vert = 
        "#version 330 core\n"

        "layout (location = 0) in vec3 aPos;\n"

        "uniform mat4 u_model;\n"
        "uniform mat4 u_view;\n"
        "uniform mat4 u_projection;\n"

        "void main()\n"
        "{\n"
        "   gl_Position = u_projection * u_view * u_model * vec4(aPos, 1.0);\n"
        "}\0";

    std::string m_shader_frag =
        "#version 330 core\n"

        "out vec4 FragColor;\n"

        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\n\0";

    std::string m_outline_shader_frag =
        "#version 330 core\n"

        "out vec4 FragColor;\n"

        "void main()\n"
        "{\n"
        "   FragColor = vec4(0.0f, 0.5f, 0.9f, 1.0f);\n"
        "}\n\0";

    m_shader = ere::shader_api::create_shader_api(m_shader_vert, m_shader_frag);
    m_outline_shader = ere::shader_api::create_shader_api(m_shader_vert, m_outline_shader_frag);
    m_camera = ere::createRef<camera_test>();

    m_camera->set_aspect_ratio(1675.0f / 820.0f);

    ere::render_api::enable_depth_testing();
    ere::render_api::set_depth_testing_function(ere::render_api::testing_function::LESS);
    ere::render_api::enable_stencil_testing();
    ere::render_api::set_stencil_function(ere::render_api::testing_function::NOTEQUAL, 1, 0xFF);
    ere::render_api::set_stencil_operation(ere::render_api::stencil_operation::KEEP, ere::render_api::stencil_operation::REPLACE, ere::render_api::stencil_operation::REPLACE);
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

    ere::render_api::enable_depth_testing();
    ere::render_api::enable_stencil_testing();
    ere::render_api::set_stencil_operation(ere::render_api::stencil_operation::KEEP, ere::render_api::stencil_operation::REPLACE, ere::render_api::stencil_operation::REPLACE);

    auto view = event.get_registry().view<renderer>();
    for (auto& e : view) {
        if (e == system_manager::get<scene_system>()->get_selected_entity()) {
            ere::render_api::set_stencil_mask(0xFF);
            ere::render_api::set_stencil_function(ere::render_api::testing_function::ALWAYS, 1, 0xFF);
        } else {
            ere::render_api::set_stencil_mask(0x00);
            ere::render_api::set_stencil_function(ere::render_api::testing_function::NOTEQUAL, 1, 0xFF);
        }

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
        } else {
            m_shader->set_uniform_mat4f("u_model", glm::mat4(1.0f));
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

    // draw the outline of the selected entity
    if (system_manager::get<scene_system>()->get_selected_entity() != entt::null) {
        auto e = system_manager::get<scene_system>()->get_selected_entity();
        if (event.get_registry().any_of<renderer>(e)) {
            ere::render_api::set_stencil_function(ere::render_api::testing_function::NOTEQUAL, 1, 0xFF);
            ere::render_api::set_stencil_mask(0x00);
            ere::render_api::disable_depth_testing();

            auto& ren = event.get_registry().get<renderer>(e);

            m_outline_shader->bind();

            if (event.get_registry().any_of<transform>(e)) {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, event.get_registry().get<transform>(e).m_position);
                model = glm::rotate(model, glm::radians(event.get_registry().get<transform>(e).m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
                model = glm::rotate(model, glm::radians(event.get_registry().get<transform>(e).m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
                model = glm::rotate(model, glm::radians(event.get_registry().get<transform>(e).m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
                model = glm::scale(model, event.get_registry().get<transform>(e).m_scale + glm::vec3({ 0.01f, 0.01f, 0.01f }));
                m_outline_shader->set_uniform_mat4f("u_model", model);
            } else {
                m_outline_shader->set_uniform_mat4f("u_model", glm::scale(glm::mat4(1.0f), glm::vec3({ 1.01f, 1.01f, 1.01f })));
            }

            ere::render_api::set_camera(m_camera);

            if (ren.m_index_buffer_api) {
                ere::render_api::draw_indexed(ren.m_vertex_array_api, m_outline_shader);
            } else {
                if (event.get_registry().any_of<mesh>(e)) {
                    ere::render_api::draw_arrays(ren.m_vertex_array_api, m_outline_shader, event.get_registry().get<mesh>(e).m_positions.size());
                }
            }
            
            ere::render_api::set_stencil_mask(0xFF);
            ere::render_api::set_stencil_function(ere::render_api::testing_function::ALWAYS, 1, 0xFF);
            ere::render_api::enable_depth_testing();
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

    static glm::vec2 d_size = { 0, 0 };
    static std::chrono::steady_clock::time_point last_resize_time = std::chrono::steady_clock::now();
    static bool has_resized = false;

    static glm::vec2 d_viewport_size = { 0, 0 };
    static std::chrono::steady_clock::time_point last_viewport_resize_time = std::chrono::steady_clock::now();
    static bool has_viewport_resized = false;

    ImVec2 imageSize = ImGui::GetContentRegionAvail();

    if (d_size.x != 0 || d_size.y != 0) {
        if (!has_resized) {
            last_resize_time = std::chrono::steady_clock::now();
            has_resized = true;
        }
    } else {
        has_resized = false;
    }

    if (d_viewport_size.x != 0 || d_viewport_size.y != 0) {
        if (!has_viewport_resized) {
            last_viewport_resize_time = std::chrono::steady_clock::now();
            has_viewport_resized = true;
        }
    } else {
        has_viewport_resized = false;
    }

    auto time_since_resize = std::chrono::steady_clock::now() - last_resize_time;
    if (has_resized && time_since_resize >= std::chrono::milliseconds(500)) {
        m_drawbuffer->resize(imageSize.x, imageSize.y);
        m_camera->set_aspect_ratio(imageSize.x / imageSize.y);
        m_drawbuffer->bind();
        ere::render_api::set_viewport({imageSize.x, imageSize.y});
        m_drawbuffer->unbind();

        has_resized = false;
    }

    auto time_since_viewport_resize = std::chrono::steady_clock::now() - last_viewport_resize_time;
    if (has_viewport_resized && time_since_viewport_resize >= std::chrono::milliseconds(200)) {
        m_drawbuffer->bind();
        ere::render_api::set_viewport({ imageSize.x, imageSize.y });
        m_drawbuffer->unbind();

        has_viewport_resized = false;
    }

    ImGui::Image((void*)(size_t)m_drawbuffer->get_color_attachemt()->get_texture_id(), imageSize);

    ImGui::End();

    d_size = { m_drawbuffer->get_width() - imageSize.x, m_drawbuffer->get_height() - imageSize.y };
    m_drawbuffer->bind();
    d_viewport_size = { ere::render_api::get_viewport().x - m_drawbuffer->get_width(), ere::render_api::get_viewport().y - m_drawbuffer->get_height() };
    m_drawbuffer->unbind();

    return false;
}

bool scene_view_system::on_update(update_event& event) {
    return false;
}

}
