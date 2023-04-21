#include <ege/ecs/systems/render_system.hpp>
#include <ege/ecs/components/renderer.hpp>
#include <ege/ecs/components/mesh.hpp>
#include <ege/ecs/components/transform.hpp>
#include <ere/api/render_api.hpp>


namespace ege {

class camera_test : public ere::camera_api {
    camera_test () {
        m_projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        m_view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    }

    const glm::mat4& get_projection_matrix() override {
        return m_projection;
    }

    const glm::mat4& get_view_matrix() override {
        return m_view;
    }

    glm::vec3 get_position() const override {
        return glm::vec3(0.0f, 0.0f, 0.0f);
    }

private:

    glm::mat4 m_projection;
    glm::mat4 m_view;
};

void render_system::on_draw(ere::ref<ere::framebuffer_api> t_drawbuffer, entt::registry& t_registery) {
    // get the material
    // for now just create a shader
    static std::string m_shader_vert = 
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

    static std::string m_shader_frag =
        "#version 330 core\n"

        "out vec4 FragColor;\n"

        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\n\0";

    static ere::ref<ere::shader_api> shader = ere::shader_api::create_shader_api(m_shader_vert, m_shader_frag);
    static ere::ref<ere::camera_api> cam = ere::createRef<camera_test>();


    // get all the entities with a renderer and mesh
    auto view = t_registery.view<renderer, mesh>();
    for (auto entity : view) {
        // get the renderer and mesh
        auto& r = view.get<renderer>(entity);
        auto& m = view.get<mesh>(entity);

        // check if the entity has a transform
        if (t_registery.any_of<transform>(entity)) {
            auto& t = t_registery.get<transform>(entity);
            // create the model matrix
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, t.m_position);
            model = glm::rotate(model, glm::radians(t.m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, glm::radians(t.m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, glm::radians(t.m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::scale(model, t.m_scale);

            // set the model matrix
            shader->set_uniform_mat4f("u_model", model);
        }

        bool any_changed = false;

        // check if the mesh positions has been updated
        if (m.m_positions_dirty || r.m_position_buffer_api == nullptr ) {
            static ere::buffer_layout layout = {
                {"aPos", ere::buffer_layout::shader_type::float_3}
            };

            // create the vertex array if it doesn't exist
            if (r.m_position_buffer_api == nullptr) {
                // create the vertex array
                r.m_position_buffer_api = ere::vertex_buffer_api::create_vertex_buffer_api(m.m_positions.data(), m.m_positions.size() * sizeof(glm::vec3));
                r.m_position_buffer_api->set_layout(layout);
            } else {
                // recreate the vertex array
                r.m_position_buffer_api = ere::vertex_buffer_api::create_vertex_buffer_api(m.m_positions.data(), m.m_positions.size() * sizeof(glm::vec3));
                r.m_position_buffer_api->set_layout(layout);
            }
            m.m_positions_dirty = false;
            any_changed = true;
        }
        //
        // // check if the mesh normals has been updated
        // if (m.m_normals_dirty || r.m_normal_buffer_api == nullptr) {
        //     static ere::buffer_layout layout = {
        //         {"aNormal", ere::buffer_layout::shader_type::float_3}
        //     };
        //
        //     // create the vertex array if it doesn't exist
        //     if (r.m_normal_buffer_api == nullptr) {
        //         // create the vertex array
        //         r.m_normal_buffer_api = ere::vertex_buffer_api::create_vertex_buffer_api(m.m_normals.data(), m.m_normals.size() * sizeof(glm::vec3));
        //         r.m_normal_buffer_api->set_layout(layout);
        //     } else {
        //         // recreate the vertex array
        //         r.m_normal_buffer_api = ere::vertex_buffer_api::create_vertex_buffer_api(m.m_normals.data(), m.m_normals.size() * sizeof(glm::vec3));
        //         r.m_normal_buffer_api->set_layout(layout);
        //     }
        //     m.m_normals_dirty = false;
        //     any_changed = true;
        // }
        //
        // // check if the mesh tangents has been updated
        // if (m.m_tangents_dirty || r.m_tangent_buffer_api == nullptr ) {
        //     static ere::buffer_layout layout = {
        //         {"aTangent", ere::buffer_layout::shader_type::float_3}
        //     };
        //
        //     // create the vertex array if it doesn't exist
        //     if (r.m_tangent_buffer_api == nullptr) {
        //         // create the vertex array
        //         r.m_tangent_buffer_api = ere::vertex_buffer_api::create_vertex_buffer_api(m.m_tangents.data(), m.m_tangents.size() * sizeof(glm::vec3));
        //         r.m_tangent_buffer_api->set_layout(layout);
        //     } else {
        //         // recreate the vertex array
        //         r.m_tangent_buffer_api = ere::vertex_buffer_api::create_vertex_buffer_api(m.m_tangents.data(), m.m_tangents.size() * sizeof(glm::vec3));
        //         r.m_tangent_buffer_api->set_layout(layout);
        //     }
        //     m.m_tangents_dirty = false;
        //     any_changed = true;
        // }
        //
        // // check if the mesh bitangents has been updated
        // if (m.m_bitangents_dirty || r.m_bitangent_buffer_api == nullptr) {
        //     static ere::buffer_layout layout = {
        //         {"aBitangent", ere::buffer_layout::shader_type::float_3}
        //     };
        //
        //     // create the vertex array if it doesn't exist
        //     if (r.m_bitangent_buffer_api == nullptr) {
        //         // create the vertex array
        //         r.m_bitangent_buffer_api = ere::vertex_buffer_api::create_vertex_buffer_api(m.m_bitangents.data(), m.m_bitangents.size() * sizeof(glm::vec3));
        //         r.m_bitangent_buffer_api->set_layout(layout);
        //     } else {
        //         // recreate the vertex array
        //         r.m_bitangent_buffer_api = ere::vertex_buffer_api::create_vertex_buffer_api(m.m_bitangents.data(), m.m_bitangents.size() * sizeof(glm::vec3));
        //         r.m_bitangent_buffer_api->set_layout(layout);
        //     }
        //     m.m_bitangents_dirty = false;
        //     any_changed = true;
        // }
        //
        // // check if the mesh uvs has been updated
        // if (m.m_uvs_dirty || r.m_uv_buffer_api == nullptr) {
        //     static ere::buffer_layout layout = {
        //         {"aTexCoord", ere::buffer_layout::shader_type::float_2}
        //     };
        //
        //     // create the vertex array if it doesn't exist
        //     if (r.m_uv_buffer_api == nullptr) {
        //         // create the vertex array
        //         r.m_uv_buffer_api = ere::vertex_buffer_api::create_vertex_buffer_api(m.m_uvs.data(), m.m_uvs.size() * sizeof(glm::vec2));
        //         r.m_uv_buffer_api->set_layout(layout);
        //     } else {
        //         // recreate the vertex array
        //         r.m_uv_buffer_api = ere::vertex_buffer_api::create_vertex_buffer_api(m.m_uvs.data(), m.m_uvs.size() * sizeof(glm::vec2));
        //         r.m_uv_buffer_api->set_layout(layout);
        //     }
        //     m.m_uvs_dirty = false;
        //     any_changed = true;
        // }

        // check if the index buffer has been updated
        if (m.m_indices_dirty || (r.m_index_buffer_api == nullptr && m.m_indices.size() > 0)) {
            // create the index buffer if it doesn't exist
            if (r.m_index_buffer_api == nullptr) {
                // create the index buffer
                r.m_index_buffer_api = ere::index_buffer_api::create_index_buffer_api(m.m_indices.data(), m.m_indices.size() * sizeof(uint32_t));
            } else {
                // recreate the index buffer
                r.m_index_buffer_api = ere::index_buffer_api::create_index_buffer_api(m.m_indices.data(), m.m_indices.size() * sizeof(uint32_t));
            }
            m.m_indices_dirty = false;
            any_changed = true;
        }

        if (any_changed) {
            r.m_vertex_array_api = ere::vertex_array_api::create_vertex_array_api();
            r.m_vertex_array_api->add_vertex_buffer(r.m_position_buffer_api);
            // r.m_vertex_array_api->add_vertex_buffer(r.m_normal_buffer_api);
            // r.m_vertex_array_api->add_vertex_buffer(r.m_tangent_buffer_api);
            // r.m_vertex_array_api->add_vertex_buffer(r.m_bitangent_buffer_api);
            // r.m_vertex_array_api->add_vertex_buffer(r.m_uv_buffer_api);

            if (r.m_index_buffer_api != nullptr) {
                r.m_vertex_array_api->set_index_buffer(r.m_index_buffer_api);
            }
        }

        ere::render_api::set_camera(cam);
        // draw the (updated) mesh
        if (r.m_index_buffer_api != nullptr) {
            ere::render_api::draw_indexed(r.m_vertex_array_api, shader);
        } else {
            ere::render_api::draw_arrays(r.m_vertex_array_api, shader, m.m_positions.size());
        }
    }
}

}
