#include <ege/ecs/systems/light_system.hpp>
#include <ege/ecs/components/point_light.hpp>
#include <ege/ecs/components/spot_light.hpp>
#include <ege/ecs/components/directional_light.hpp>
#include <ege/ecs/components/area_light.hpp>

#include <ege/ecs/components/transform.hpp>
#include <ege/ecs/components/material.hpp>

namespace ege {

bool light_system::on_pre_draw(pre_draw_event &event) {
    auto mat_view = event.get_registry().view<material>();
    auto point_view = event.get_registry().view<point_light>();
    int i = 0;

    // point lights
    for (auto entity : point_view) {
        auto &light = point_view.get<point_light>(entity);
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);

        if (event.get_registry().any_of<transform>(entity)) {
            auto &trans = event.get_registry().get<transform>(entity);
            position = trans.m_position;
        }

        for (auto e : mat_view) {
            auto &mat = mat_view.get<material>(e);
            if (mat.m_active_shader) {
                mat.m_active_shader->set_uniform_3f("u_point_lights[" + std::to_string(i) + "].position", position);
                mat.m_active_shader->set_uniform_3f("u_point_lights[" + std::to_string(i) + "].color", light.m_color);
                mat.m_active_shader->set_uniform_1f("u_point_lights[" + std::to_string(i) + "].intensity", light.m_intensity);
            }
        }
        i++;
    }

    for (auto entity : mat_view) {
        auto& mat = mat_view.get<material>(entity);
        mat.m_active_shader->set_uniform_1i("u_num_point_lights", i);
    }

    // spot lights
    auto spot_view = event.get_registry().view<spot_light>();
    i = 0;

    for (auto entity : spot_view) {
        auto &light = spot_view.get<spot_light>(entity);

        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 direction = glm::vec3(0.0f, 0.0f, 0.0f);
        if (event.get_registry().any_of<transform>(entity)) {
            auto &trans = event.get_registry().get<transform>(entity);
            position = trans.m_position;

            direction.x = sin(trans.m_rotation.y);
            direction.y = -(sin(trans.m_rotation.x)*cos(trans.m_rotation.y));
            direction.z = -(cos(trans.m_rotation.x)*cos(trans.m_rotation.y));
        }

        for (auto e : mat_view) {
            auto &mat = mat_view.get<material>(e);
            if (mat.m_active_shader) {
                mat.m_active_shader->set_uniform_3f("u_spot_lights[" + std::to_string(i) + "].position", position);
                mat.m_active_shader->set_uniform_3f("u_spot_lights[" + std::to_string(i) + "].direction", direction);
                mat.m_active_shader->set_uniform_3f("u_spot_lights[" + std::to_string(i) + "].color", light.color);
                mat.m_active_shader->set_uniform_1f("u_spot_lights[" + std::to_string(i) + "].intensity", 1);
                mat.m_active_shader->set_uniform_1f("u_spot_lights[" + std::to_string(i) + "].inner_cutoff", light.cutoff);
                mat.m_active_shader->set_uniform_1f("u_spot_lights[" + std::to_string(i) + "].outer_cutoff", light.outer_cutoff);
            }
        }
        i++;
    }

    for (auto entity : mat_view) {
        auto& mat = mat_view.get<material>(entity);
        mat.m_active_shader->set_uniform_1i("u_num_spot_lights", i);
    }

    // directional lights
    auto directional_view = event.get_registry().view<directional_light>();
    i = 0;

    for (auto entity : directional_view) {
        auto &light = directional_view.get<directional_light>(entity);

        glm::vec3 direction = glm::vec3(0.0f, 0.0f, 0.0f);
        if (event.get_registry().any_of<transform>(entity)) {
            auto &trans = event.get_registry().get<transform>(entity);
            direction.x = sin(trans.m_rotation.y);
            direction.y = -(sin(trans.m_rotation.x)*cos(trans.m_rotation.y));
            direction.z = -(cos(trans.m_rotation.x)*cos(trans.m_rotation.y));
        }

        for (auto e : mat_view) {
            auto &mat = mat_view.get<material>(e);
            if (mat.m_active_shader) {
                mat.m_active_shader->set_uniform_3f("u_directional_lights[" + std::to_string(i) + "].direction", direction);
                mat.m_active_shader->set_uniform_3f("u_directional_lights[" + std::to_string(i) + "].color", light.color);
                mat.m_active_shader->set_uniform_1f("u_directional_lights[" + std::to_string(i) + "].intensity", light.intensity);
            }
        }
        i++;
    }

    for (auto entity : mat_view) {
        auto& mat = mat_view.get<material>(entity);
        mat.m_active_shader->set_uniform_1i("u_num_directional_lights", i);
    }

    // area lights
    auto area_view = event.get_registry().view<area_light>();
    i = 0;

    for (auto entity : area_view) {
        auto &light = area_view.get<area_light>(entity);

        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
        if (event.get_registry().any_of<transform>(entity)) {
            auto &trans = event.get_registry().get<transform>(entity);
            position = trans.m_position;
        }

        for (auto e : mat_view) {
            auto &mat = mat_view.get<material>(e);

            if (mat.m_active_shader) {
                mat.m_active_shader->set_uniform_3f("u_area_lights[" + std::to_string(i) + "].position", position);
                mat.m_active_shader->set_uniform_3f("u_area_lights[" + std::to_string(i) + "].normals", light.m_normal);
                mat.m_active_shader->set_uniform_3f("u_area_lights[" + std::to_string(i) + "].color", light.m_color);
                mat.m_active_shader->set_uniform_1f("u_area_lights[" + std::to_string(i) + "].intensity", light.m_intensity);
                mat.m_active_shader->set_uniform_1f("u_area_lights[" + std::to_string(i) + "].width", light.m_width);
                mat.m_active_shader->set_uniform_1f("u_area_lights[" + std::to_string(i) + "].height", light.m_height);
            }
        }
        i++;
    }

    for (auto entity : mat_view) {
        auto& mat = mat_view.get<material>(entity);
        mat.m_active_shader->set_uniform_1i("u_num_area_lights", i);
    }

    return false;
}

}
