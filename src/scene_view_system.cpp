#include <ege/ecs/systems/scene_view_system.hpp>
#include <ege/core/logger.hpp>
#include <ere/api/render_api.hpp>
#include <ege/ecs/components/renderer.hpp>
#include <ege/ecs/components/transform.hpp>
#include <ege/ecs/components/mesh.hpp>
#include <ege/ecs/systems/scene_camera_system.hpp>
#include <ege/ecs/components/skybox.hpp>
#include <ege/ecs/components/material.hpp>

#include <imgui.h>

namespace ege {


scene_view_system::scene_view_system() {
    create_grid();

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

    // set camera
    ere::render_api::set_camera(system_manager::get<scene_camera_system>()->get_camera(event.get_registry()).m_camera);

    // draw skybox
    auto cam = system_manager::get<scene_camera_system>()->get_camera_entity(event.get_registry());
    if (event.get_registry().any_of<skybox>(cam)) {
        auto& sky = event.get_registry().get<skybox>(cam);

        if (sky.m_cubemap != nullptr) {
            ere::render_api::disable_depth_test_write();
            ere::render_api::draw_arrays_textured(sky.m_vao, sky.m_shader, sky.m_positions.size(), { sky.m_cubemap });
            ere::render_api::enable_depth_test_write();
        }
    }

    auto view = event.get_registry().view<renderer, material>();
    for (auto& e : view) {
        if (e == system_manager::get<scene_system>()->get_selected_entity()) {
            ere::render_api::set_stencil_mask(0xFF);
            ere::render_api::set_stencil_function(ere::render_api::testing_function::ALWAYS, 1, 0xFF);
        } else {
            ere::render_api::set_stencil_mask(0x00);
            ere::render_api::set_stencil_function(ere::render_api::testing_function::ALWAYS, 1, 0xFF);
        }

        auto& ren = view.get<renderer>(e);
        auto& mat = view.get<material>(e);

        mat.m_active_shader->bind();

        if (event.get_registry().any_of<transform>(e)) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, event.get_registry().get<transform>(e).m_position);
            model = glm::rotate(model, glm::radians(event.get_registry().get<transform>(e).m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, glm::radians(event.get_registry().get<transform>(e).m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, glm::radians(event.get_registry().get<transform>(e).m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::scale(model, event.get_registry().get<transform>(e).m_scale);
            mat.m_active_shader->set_uniform_mat4f("u_model", model);
        } else {
            mat.m_active_shader->set_uniform_mat4f("u_model", glm::mat4(1.0f));
        }

        ere::render_api::set_camera(system_manager::get<scene_camera_system>()->get_camera(event.get_registry()).m_camera);

        if (ren.m_index_buffer_api) {
            ere::render_api::draw_indexed(ren.m_vertex_array_api, mat.m_active_shader);
        } else {
            if (event.get_registry().any_of<mesh>(e)) {
                ere::render_api::draw_arrays(ren.m_vertex_array_api, mat.m_active_shader, event.get_registry().get<mesh>(e).m_positions.size());
            }
        }

    }

    ere::render_api::set_stencil_mask(0x00);
    // ere::render_api::set_stencil_function(ere::render_api::testing_function::NOTEQUAL, 1, 0xFF);
    ere::render_api::set_stencil_function(ere::render_api::testing_function::ALWAYS, 1, 0xFF);
    // draw grid
    m_grid.shader->set_uniform_mat4f("u_model", m_grid.transform);
    ere::render_api::set_camera(system_manager::get<scene_camera_system>()->get_camera(event.get_registry()).m_camera);
    ere::render_api::draw_indexed_textured(m_grid.vao, m_grid.shader, { m_grid.tex });

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

            ere::render_api::set_camera(system_manager::get<scene_camera_system>()->get_camera(event.get_registry()).m_camera);

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

    bool is_focused = ImGui::IsWindowFocused();
    system_manager::get<scene_camera_system>()->set_focused(is_focused);

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
        system_manager::get<scene_camera_system>()->get_camera(event.get_registry()).m_aspect_ratio = imageSize.x / imageSize.y;
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

void scene_view_system::create_grid() {
    m_grid.vao = ere::vertex_array_api::create_vertex_array_api();

    m_grid.vertices = {
        // positions
        glm::vec3(-1.0f, 0.0f, -1.0f),
        glm::vec3(-1.0f, 0.0f,  1.0f),
        glm::vec3(1.0f, 0.0f,  1.0f),
        glm::vec3(1.0f, 0.0f, -1.0f)
    };

    m_grid.indices = {
        0, 1, 2,
        2, 3, 0
    };

    m_grid.uvs = {
        glm::vec2(0.0f, 0.0f),
        glm::vec2(0.0f, 1.0f),
        glm::vec2(1.0f, 1.0f),
        glm::vec2(1.0f, 0.0f)
    };

    float grid_scale = 1000.f;
    std::for_each(m_grid.uvs.begin(), m_grid.uvs.end(), [&](glm::vec2& v) {
        v *= grid_scale / 10;
    });

    m_grid.transform = glm::mat4(1.0f);
    m_grid.transform = glm::scale(m_grid.transform, glm::vec3(grid_scale));

    ere::buffer_layout positions_layout = {
        {"aPos", ere::buffer_layout::shader_type::float_3}
    };

    ere::buffer_layout uvs_layout = {
        {"aUV", ere::buffer_layout::shader_type::float_2}
    };

    m_grid.vbo_pos = ere::vertex_buffer_api::create_vertex_buffer_api(m_grid.vertices.data(), m_grid.vertices.size() * sizeof(glm::vec3));
    m_grid.vbo_pos->set_layout(positions_layout);
    m_grid.vbo_uv = ere::vertex_buffer_api::create_vertex_buffer_api(m_grid.uvs.data(), m_grid.uvs.size() * sizeof(glm::vec2));
    m_grid.vbo_uv->set_layout(uvs_layout);
    m_grid.ibo = ere::index_buffer_api::create_index_buffer_api(m_grid.indices.data(), m_grid.indices.size() * sizeof(uint32_t));

    m_grid.vao->add_vertex_buffer(m_grid.vbo_pos);
    m_grid.vao->add_vertex_buffer(m_grid.vbo_uv);
    m_grid.vao->set_index_buffer(m_grid.ibo);

    m_grid.tex = ere::texture2d_api::create_texture2d_api("res/textures/scene_view_grid_floor.png");
    m_grid.tex->set_wrap(ere::texture2d_api::wrap::REPEAT);
    m_grid.tex->set_uniform_name("floorTex");

    m_grid.shader = ere::shader_api::create_shader_api_from_file("res/shaders/scene_view_floor/vert.glsl", "res/shaders/scene_view_floor/frag.glsl");
}

}
