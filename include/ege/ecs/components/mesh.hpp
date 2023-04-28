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
    capsule = 2,
    cylinder = 3,
    plane = 4,
    donut = 5,
    custom = 6,
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

        m.m_uvs = {
            // front
            glm::vec2(0, 0),
            glm::vec2(1, 0),
            glm::vec2(1, 1),
            glm::vec2(0, 1),
            // back
            glm::vec2(1, 0),
            glm::vec2(0, 0),
            glm::vec2(0, 1),
            glm::vec2(1, 1),

            // left
            glm::vec2(0, 0),
            glm::vec2(1, 0),
            glm::vec2(1, 1),
            glm::vec2(0, 1),

            // right
            glm::vec2(1, 0),
            glm::vec2(0, 0),
            glm::vec2(0, 1),
            glm::vec2(1, 1),

            // top
            glm::vec2(0, 1),
            glm::vec2(1, 1),
            glm::vec2(1, 0),
            glm::vec2(0, 0),

            // bottom
            glm::vec2(0, 0),
            glm::vec2(1, 0),
            glm::vec2(1, 1),
            glm::vec2(0, 1),
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

        const unsigned int X_SEGMENTS = 64;
        const unsigned int Y_SEGMENTS = 64;
        const float PI = 3.14159265359f;
        for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
        {
            for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
            {
                float xSegment = (float)x / (float)X_SEGMENTS;
                float ySegment = (float)y / (float)Y_SEGMENTS;
                float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
                float yPos = std::cos(ySegment * PI);
                float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

                m.m_positions.push_back(glm::vec3(xPos, yPos, zPos));
                m.m_uvs.push_back(glm::vec2(xSegment, ySegment));
                m.m_normals.push_back(glm::vec3(xPos, yPos, zPos));
            }
        }

        // calculate indices for each 3 point triangle
        for (unsigned int x = 0; x < X_SEGMENTS; ++x)
        {
            for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
            {
                unsigned int first = (x * (Y_SEGMENTS + 1)) + y;
                unsigned int second = first + Y_SEGMENTS + 1;
                m.m_indices.push_back(first);
                m.m_indices.push_back(second);
                m.m_indices.push_back(first + 1);

                m.m_indices.push_back(second);
                m.m_indices.push_back(second + 1);
                m.m_indices.push_back(first + 1);
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

    static mesh generate_capsule_mesh() {
        mesh m;
        m.m_type = mesh_type::capsule;

        return m;
    }

    static mesh generate_cylinder_mesh() {
        mesh m;
        m.m_type = mesh_type::cylinder;

        return m;
    }

    static mesh generate_plane_mesh(glm::vec2 t_size) {
        mesh m;
        m.m_type = mesh_type::plane;

        m.m_positions = {
            glm::vec3(-t_size.x, 0.0f, -t_size.y),
            glm::vec3(t_size.x, 0.0f, -t_size.y),
            glm::vec3(t_size.x, 0.0f, t_size.y),
            glm::vec3(-t_size.x, 0.0f, t_size.y)
        };

        m.m_uvs = {
            glm::vec2(0, 0),
            glm::vec2(1, 0),
            glm::vec2(1, 1),
            glm::vec2(0, 1)
        };

        m.m_normals = {
            glm::vec3(0, 1, 0),
            glm::vec3(0, 1, 0),
            glm::vec3(0, 1, 0),
            glm::vec3(0, 1, 0)
        };

        m.m_indices = {
            0, 1, 2, 0, 2, 3
        };

        m.m_positions_dirty = true;
        m.m_positions_size_dirty = true;
        m.m_normals_dirty = true;
        m.m_normals_size_dirty = true;
        m.m_uvs_dirty = true;
        m.m_uvs_size_dirty = true;
        m.m_indices_dirty = true;
        m.m_indices_size_dirty = true;

        return m;
    }

    static mesh generate_donut_mesh() {
        mesh m;
        m.m_type = mesh_type::donut;

        return m;
    }

};


template<>
inline void inspector_system::component_editor_widget<mesh>(entt::registry& t_registery, entt::entity t_entity) {
    auto& m = t_registery.get<mesh>(t_entity);

    static const char* items[] = { "Cube", "Sphere", "Capsule", "Cylinder", "Plane", "Donut", "Custom" };
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
        if (ImGui::Selectable(items[static_cast<int>(mesh_type::capsule)], m.m_type == mesh_type::capsule)) {
            m = mesh_generator::generate_capsule_mesh();
        }
        if (ImGui::Selectable(items[static_cast<int>(mesh_type::cylinder)], m.m_type == mesh_type::cylinder)) {
            m = mesh_generator::generate_cylinder_mesh();
        }
        if (ImGui::Selectable(items[static_cast<int>(mesh_type::plane)], m.m_type == mesh_type::plane)) {
            m = mesh_generator::generate_plane_mesh(glm::vec2(1.0f));
        }
        if (ImGui::Selectable(items[static_cast<int>(mesh_type::donut)], m.m_type == mesh_type::donut)) {
            m = mesh_generator::generate_donut_mesh();
        }

        ImGui::EndCombo();
    }
    ImGui::PopID();
}


}


#endif // __EGE_MESH_HPP__
