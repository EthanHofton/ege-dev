#ifndef __EGE_SKYBOX_HPP__
#define __EGE_SKYBOX_HPP__

#include <ere/api/cubemap_api.hpp>
#include <ere/api/shader_api.hpp>
#include <ere/api/vertex_array_api.hpp>
#include <ege/ecs/systems/inspector_system.hpp>

namespace ege {

struct skybox {
    ere::ref<ere::cubemap_api> m_cubemap;
    ere::ref<ere::shader_api> m_shader;

    std::vector<glm::vec3> m_positions;

    ere::ref<ere::vertex_array_api> m_vao;
    ere::ref<ere::vertex_buffer_api> m_vbo_pos;
};

struct skybox_generator {
    static skybox generate(std::vector<std::string> t_paths) {
        skybox s;

        s.m_shader = ere::shader_api::create_shader_api_from_file("res/shaders/skybox/vert.glsl", "res/shaders/skybox/frag.glsl");
        s.m_cubemap = ere::cubemap_api::create_cubemap_api(t_paths);
        s.m_cubemap->set_uniform_name("skybox");

        s.m_positions = {
            // positions          
            glm::vec3(-1.0f,  1.0f, -1.0f),
            glm::vec3(-1.0f, -1.0f, -1.0f),
            glm::vec3( 1.0f, -1.0f, -1.0f),
            glm::vec3( 1.0f, -1.0f, -1.0f),
            glm::vec3( 1.0f,  1.0f, -1.0f),
            glm::vec3(-1.0f,  1.0f, -1.0f),

            glm::vec3(-1.0f, -1.0f,  1.0f),
            glm::vec3(-1.0f, -1.0f, -1.0f),
            glm::vec3(-1.0f,  1.0f, -1.0f),
            glm::vec3(-1.0f,  1.0f, -1.0f),
            glm::vec3(-1.0f,  1.0f,  1.0f),
            glm::vec3(-1.0f, -1.0f,  1.0f),

            glm::vec3( 1.0f, -1.0f, -1.0f),
            glm::vec3( 1.0f, -1.0f,  1.0f),
            glm::vec3( 1.0f,  1.0f,  1.0f),
            glm::vec3( 1.0f,  1.0f,  1.0f),
            glm::vec3( 1.0f,  1.0f, -1.0f),
            glm::vec3( 1.0f, -1.0f, -1.0f),

            glm::vec3(-1.0f, -1.0f,  1.0f),
            glm::vec3(-1.0f,  1.0f,  1.0f),
            glm::vec3( 1.0f,  1.0f,  1.0f),
            glm::vec3( 1.0f,  1.0f,  1.0f),
            glm::vec3( 1.0f, -1.0f,  1.0f),
            glm::vec3(-1.0f, -1.0f,  1.0f),

            glm::vec3(-1.0f,  1.0f, -1.0f),
            glm::vec3( 1.0f,  1.0f, -1.0f),
            glm::vec3( 1.0f,  1.0f,  1.0f),
            glm::vec3( 1.0f,  1.0f,  1.0f),
            glm::vec3(-1.0f,  1.0f,  1.0f),
            glm::vec3(-1.0f,  1.0f, -1.0f),

            glm::vec3(-1.0f, -1.0f, -1.0f),
            glm::vec3(-1.0f, -1.0f,  1.0f),
            glm::vec3( 1.0f, -1.0f, -1.0f),
            glm::vec3( 1.0f, -1.0f, -1.0f),
            glm::vec3(-1.0f, -1.0f,  1.0f),
            glm::vec3( 1.0f, -1.0f,  1.0f)
        };

        s.m_vao = ere::vertex_array_api::create_vertex_array_api();
        s.m_vbo_pos = ere::vertex_buffer_api::create_vertex_buffer_api(s.m_positions.data(), s.m_positions.size() * sizeof(glm::vec3));
        s.m_vbo_pos->set_layout({
            { "a_Pos", ere::buffer_layout::shader_type::float_3 }
        });

        s.m_vao->add_vertex_buffer(s.m_vbo_pos);

        return s;
    }
};

template<>
inline void inspector_system::component_editor_widget<skybox>(entt::registry& t_registery, entt::entity t_entity) {
    auto& s = t_registery.get<skybox>(t_entity);

    ImGui::PushID(std::to_string((int)t_entity).c_str());

    if (ImGui::Button("Load Skybox")) {
        std::vector<std::string> paths = {
            "res/skybox/default_skybox/right.jpg",
            "res/skybox/default_skybox/left.jpg",
            "res/skybox/default_skybox/top.jpg",
            "res/skybox/default_skybox/bottom.jpg",
            "res/skybox/default_skybox/front.jpg",
            "res/skybox/default_skybox/back.jpg"
        };

        s = skybox_generator::generate(paths);
    }

    ImGui::PopID();
}

}

#endif // __EGE_SKYBOX_HPP__
