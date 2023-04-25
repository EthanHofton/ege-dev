#ifndef __EGE_MESH_HPP__
#define __EGE_MESH_HPP__

#include <vector>
#include <glm/glm.hpp>
#include <ege/ecs/systems/inspector_system.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace ege {

enum class mesh_type {
    cube = 0,
    sphere = 1,
    custom = 2,
};

struct mesh {
    std::vector<glm::vec3> m_positions;
    bool m_positions_dirty = false;
    bool m_positions_size_dirty = false;
    std::vector<glm::vec2> m_uvs;
    bool m_uvs_dirty = false;
    bool m_uvs_size_dirty = false;
    std::vector<glm::vec3> m_normals;
    bool m_normals_dirty = false;
    bool m_normals_size_dirty = false;
    std::vector<glm::vec3> m_tangents;
    bool m_tangents_dirty = false;
    bool m_tangents_size_dirty = false;
    std::vector<glm::vec3> m_bitangents;
    bool m_bitangents_dirty = false;
    bool m_bitangents_size_dirty = false;
    std::vector<uint32_t> m_indices;
    bool m_indices_dirty = false;
    bool m_indices_size_dirty = false;
    mesh_type m_type = mesh_type::custom;
};

struct mesh_generator {
    static mesh generate_cube_mesh(const glm::vec3& t_dim) {
        mesh m;
        m.m_type = mesh_type::cube;

        m.m_positions = {
            // front
            glm::vec3(-t_dim.x, -t_dim.y, t_dim.z),
            glm::vec3(t_dim.x, -t_dim.y, t_dim.z),
            glm::vec3(t_dim.x, t_dim.y, t_dim.z),
            glm::vec3(-t_dim.x, t_dim.y, t_dim.z),
            // back
            glm::vec3(-t_dim.x, -t_dim.y, -t_dim.z),
            glm::vec3(t_dim.x, -t_dim.y, -t_dim.z),
            glm::vec3(t_dim.x, t_dim.y, -t_dim.z),
            glm::vec3(-t_dim.x, t_dim.y, -t_dim.z),

            // left
            glm::vec3(-t_dim.x, -t_dim.y, t_dim.z),
            glm::vec3(-t_dim.x, -t_dim.y, -t_dim.z),
            glm::vec3(-t_dim.x, t_dim.y, -t_dim.z),
            glm::vec3(-t_dim.x, t_dim.y, t_dim.z),

            // right
            glm::vec3(t_dim.x, -t_dim.y, t_dim.z),
            glm::vec3(t_dim.x, -t_dim.y, -t_dim.z),
            glm::vec3(t_dim.x, t_dim.y, -t_dim.z),
            glm::vec3(t_dim.x, t_dim.y, t_dim.z),

            // top
            glm::vec3(-t_dim.x, t_dim.y, t_dim.z),
            glm::vec3(t_dim.x, t_dim.y, t_dim.z),
            glm::vec3(t_dim.x, t_dim.y, -t_dim.z),
            glm::vec3(-t_dim.x, t_dim.y, -t_dim.z),

            // bottom
            glm::vec3(-t_dim.x, -t_dim.y, t_dim.z),
            glm::vec3(t_dim.x, -t_dim.y, t_dim.z),
            glm::vec3(t_dim.x, -t_dim.y, -t_dim.z),
            glm::vec3(-t_dim.x, -t_dim.y, -t_dim.z),
        };

        m.m_normals = {
            // front
            glm::vec3(0, 0, 1),
            glm::vec3(0, 0, 1),
            glm::vec3(0, 0, 1),
            glm::vec3(0, 0, 1),
            // back
            glm::vec3(0, 0, -1),
            glm::vec3(0, 0, -1),
            glm::vec3(0, 0, -1),
            glm::vec3(0, 0, -1),

            // left
            glm::vec3(-1, 0, 0),
            glm::vec3(-1, 0, 0),
            glm::vec3(-1, 0, 0),
            glm::vec3(-1, 0, 0),

            // right
            glm::vec3(1, 0, 0),
            glm::vec3(1, 0, 0),
            glm::vec3(1, 0, 0),
            glm::vec3(1, 0, 0),

            // top
            glm::vec3(0, 1, 0),
            glm::vec3(0, 1, 0),
            glm::vec3(0, 1, 0),
            glm::vec3(0, 1, 0),

            // bottom
            glm::vec3(0, -1, 0),
            glm::vec3(0, -1, 0),
            glm::vec3(0, -1, 0),
            glm::vec3(0, -1, 0),
        };

        m.m_indices = {
            0, 1, 2, 0, 2, 3, // front
            4, 5, 6, 4, 6, 7, // back
            8, 9, 10, 8, 10, 11, // left
            12, 13, 14, 12, 14, 15, // right
            16, 17, 18, 16, 18, 19, // top
            20, 21, 22, 20, 22, 23 // bottom
        };

        m.m_positions_dirty = true;
        m.m_positions_size_dirty = true;
        m.m_normals_dirty = true;
        m.m_normals_size_dirty = true;
        m.m_indices_dirty = true;
        m.m_indices_size_dirty = true;

        return m;
    }

    static mesh generate_sphere_mesh(float t_radius) {
        mesh m;
        m.m_type = mesh_type::sphere;

        int numSlices = 50;
        int numStacks = 50;
        float phiStep = glm::two_pi<float>() / numSlices;
        float thetaStep = glm::pi<float>() / numStacks;
        // Generate vertices
        for (int i = 0; i <= numStacks; i++) {
            float theta = i * thetaStep;
            float sinTheta = glm::sin(theta);
            float cosTheta = glm::cos(theta);

            for (int j = 0; j <= numSlices; j++) {
                float phi = j * phiStep;
                float sinPhi = glm::sin(phi);
                float cosPhi = glm::cos(phi);

                glm::vec3 vertex = glm::vec3(cosPhi * sinTheta, cosTheta, sinPhi * sinTheta) * t_radius;
                m.m_positions.push_back(vertex);
                m.m_normals.push_back(glm::normalize(vertex));
                m.m_tangents.push_back(glm::vec3(-sinPhi, 0, cosPhi));
                m.m_bitangents.push_back(glm::cross(m.m_normals.back(), m.m_tangents.back()));
                m.m_uvs.push_back(glm::vec2(phi / glm::two_pi<float>(), theta / glm::pi<float>()));

                int vertexIndex = i * (numSlices + 1) + j;

                m.m_indices.push_back(vertexIndex);
                m.m_indices.push_back(vertexIndex + numSlices + 1);
                m.m_indices.push_back(vertexIndex + 1);

                m.m_indices.push_back(vertexIndex + 1);
                m.m_indices.push_back(vertexIndex + numSlices + 1);
                m.m_indices.push_back(vertexIndex + numSlices + 2);
            }
        }

        m.m_positions_dirty = true;
        m.m_positions_size_dirty = true;
        m.m_normals_dirty = true;
        m.m_normals_size_dirty = true;
        m.m_tangents_dirty = true;
        m.m_tangents_size_dirty = true;
        m.m_bitangents_dirty = true;
        m.m_bitangents_size_dirty = true;
        m.m_uvs_dirty = true;
        m.m_uvs_size_dirty = true;
        m.m_indices_dirty = true;
        m.m_indices_size_dirty = true;

        return m;
    }
};


template<>
inline void inspector_system::component_editor_widget<mesh>(entt::registry& t_registery, entt::entity t_entity) {
    auto& m = t_registery.get<mesh>(t_entity);

    static const char* items[] = { "Cube", "Sphere", "Custom" };
    const char* current_item = items[static_cast<int>(m.m_type)];
    ImGui::PushID("mesh_type");
    if (ImGui::BeginCombo("Mesh", current_item)) {
        if (ImGui::Selectable(items[static_cast<int>(mesh_type::cube)], m.m_type == mesh_type::cube)) {
            m = mesh_generator::generate_cube_mesh(glm::vec3(1.0f));
        }
        if (ImGui::Selectable(items[static_cast<int>(mesh_type::sphere)], m.m_type == mesh_type::sphere)) {
            m = mesh_generator::generate_sphere_mesh(1);
        }
        if (ImGui::Selectable(items[static_cast<int>(mesh_type::custom)], m.m_type == mesh_type::custom)) {
            m.m_type = mesh_type::custom;
        }
        ImGui::EndCombo();
    }
    ImGui::PopID();
}


}


#endif // __EGE_MESH_HPP__
