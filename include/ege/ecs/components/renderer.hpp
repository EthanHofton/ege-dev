#ifndef __EGE_RENDERER_HPP__
#define __EGE_RENDERER_HPP__

#include <ere/api/vertex_array_api.hpp>
#include <ege/ecs/systems/inspector_system.hpp>

namespace ege {

struct renderer {
    ere::ref<ere::vertex_array_api> m_vertex_array_api;
    ere::ref<ere::vertex_buffer_api> m_position_buffer_api;
    ere::ref<ere::vertex_buffer_api> m_normal_buffer_api;
    ere::ref<ere::vertex_buffer_api> m_tangent_buffer_api;
    ere::ref<ere::vertex_buffer_api> m_bitangent_buffer_api;
    ere::ref<ere::vertex_buffer_api> m_uv_buffer_api;
    ere::ref<ere::index_buffer_api> m_index_buffer_api;
};

template<>
inline void inspector_system::component_editor_widget<renderer>(entt::registry& t_registery, entt::entity t_entity) {
    auto& r = t_registery.get<renderer>(t_entity);
    ImGui::Text("Renderer");
}

}


#endif // __EGE_RENDERER_HPP__
