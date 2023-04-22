#include <ege/ecs/systems/render_buffers_system.hpp>
#include <ege/ecs/components/renderer.hpp>
#include <ege/ecs/components/mesh.hpp>

namespace ege {

bool render_buffers_system::on_pre_draw(pre_draw_event& event) {
    auto view = event.get_registry().view<renderer, mesh>();

    for (auto entity : view) {
        bool buffer_changed = false;
        auto& r = view.get<renderer>(entity);
        auto& m = view.get<mesh>(entity);

        static ere::buffer_layout positions_layout = {
            {"aPos", ere::buffer_layout::shader_type::float_3}
        };
        
        // check if the mesh positions has been updated
        if (m.m_positions.size() != 0) {
            if (m.m_positions_size_dirty || r.m_position_buffer_api == nullptr) {
                r.m_position_buffer_api = ere::vertex_buffer_api::create_vertex_buffer_api(m.m_positions.data(), m.m_positions.size() * sizeof(glm::vec3));
                r.m_position_buffer_api->set_layout(positions_layout);
                m.m_positions_size_dirty = false;
                m.m_positions_dirty = false;
                buffer_changed = true;
            } else if (m.m_positions_dirty) {
                r.m_position_buffer_api->set_data(m.m_positions.data(), m.m_positions.size() * sizeof(glm::vec3));
                m.m_positions_dirty = false;
                buffer_changed = true;
            }
        }

        // check if the mesh indices have been updated
        if (m.m_indices.size() != 0) {
            if (m.m_indices_size_dirty || r.m_index_buffer_api == nullptr) {
                r.m_index_buffer_api = ere::index_buffer_api::create_index_buffer_api(m.m_indices.data(), m.m_indices.size() * sizeof(uint32_t));
                m.m_indices_size_dirty = false;
                m.m_indices_dirty = false;
                buffer_changed = true;
            } else if (m.m_indices_dirty) {
                r.m_index_buffer_api->set_data(m.m_indices.data(), m.m_indices.size() * sizeof(uint32_t));
                m.m_indices_dirty = false;
                buffer_changed = true;
            }
        }

        if (buffer_changed || r.m_vertex_array_api == nullptr) {
            r.m_vertex_array_api = ere::vertex_array_api::create_vertex_array_api();
            r.m_vertex_array_api->add_vertex_buffer(r.m_position_buffer_api);
            r.m_vertex_array_api->set_index_buffer(r.m_index_buffer_api);
        }
    }

    return false;
}

}
