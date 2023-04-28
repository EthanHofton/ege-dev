#include <ege/ecs/systems/scene_camera_system.hpp>
#include <ege/ecs/components/scene_camera.hpp>
#include <ege/ecs/components/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <ege/ecs/components/UUID.hpp>
#include <ege/ecs/components/tag.hpp>
#include <ere/core/application.hpp>
#include <ere/mappings/mouse_map.hpp>
#include <ere/mappings/key_map.hpp>
#include <ege/core/logger.hpp>
#include <ege/ecs/components/skybox.hpp>

namespace ege {

bool scene_camera_system::on_update(ege::update_event& e) {
    if (!m_focused) return false;

    auto camera_entity = get_camera_entity(e.get_registry());
    auto& camera = get_camera(e.get_registry());

    update_camera(e.get_registry(), camera_entity, camera);

    return false;
}

bool scene_camera_system::on_mouse_scrolled(mouse_scrolled_event& e) {
    if (!m_focused) return false;

    float move_speed = 3.f;
    auto camera_entity = get_camera_entity(e.get_registry());
    auto& camera = get_camera(e.get_registry());

    if (e.get_registry().any_of<ege::transform>(camera_entity)) {
        float dt = ere::application::get_application()->get_delta_time();
        glm::vec3 front = camera.m_front;

        e.get_registry().get<transform>(camera_entity).m_position.x += front.x * e.get_offset().y * dt * move_speed;
        e.get_registry().get<transform>(camera_entity).m_position.y += front.y * e.get_offset().y * dt * move_speed;
        e.get_registry().get<transform>(camera_entity).m_position.z += front.z * e.get_offset().y * dt * move_speed;
    }

    return false;
}

bool scene_camera_system::on_mouse_moved(mouse_moved_event &event) {
    if (!m_focused) return false;

    float move_speed = 3.f;
    float orbit_speed = 10.f;
    float orbit_distance = 30;

    if (ere::application::get_application()->is_mouse_button_pressed(ERE_MOUSE_BUTTON_1)) {
        auto camera_entity = get_camera_entity(event.get_registry());
        auto& camera = get_camera(event.get_registry());

        if (ere::application::get_application()->is_key_pressed(ERE_KEY_LEFT_ALT)) {
            // orbit
            float dt = ere::application::get_application()->get_delta_time();

            if (event.get_registry().any_of<ege::transform>(camera_entity)) {
                auto& trans = event.get_registry().get<ege::transform>(camera_entity);
                glm::vec3 pivot_point = trans.m_position + camera.m_front * orbit_distance;

                trans.m_rotation.x += event.get_delta_pos().y * dt * orbit_speed;
                trans.m_rotation.y -= event.get_delta_pos().x * dt * orbit_speed;

                float yaw = trans.m_rotation.y;
                float pitch = trans.m_rotation.x;

                glm::vec3 front;
                front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
                front.y = sin(glm::radians(pitch));
                front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

                camera.m_front = glm::normalize(front);
                camera.m_right = glm::normalize(glm::cross(camera.m_front, {0.0f, 1.0f, 0.0f}));
                camera.m_up = glm::normalize(glm::cross(camera.m_right, camera.m_front));

                trans.m_position = pivot_point - camera.m_front * orbit_distance;
            }

        } else {
            // grab and move
            if (event.get_registry().any_of<ege::transform>(camera_entity)) {
                float dt = ere::application::get_application()->get_delta_time();
                auto& trans = event.get_registry().get<ege::transform>(camera_entity);

                trans.m_position += (camera.m_right * (event.get_delta_pos().x * -move_speed) - camera.m_up * (event.get_delta_pos().y * -move_speed)) * dt;
            }
        }
    } else if (ere::application::get_application()->is_mouse_button_pressed(ERE_MOUSE_BUTTON_2)) {
        auto camera_entity = get_camera_entity(event.get_registry());
        auto& camera = get_camera(event.get_registry());

        if (ere::application::get_application()->is_key_pressed (ERE_KEY_LEFT_ALT)) {
            // zoom in direction of camera
            // zoom speed should be independent of the direction of the scroll
            if (event.get_registry().any_of<ege::transform>(camera_entity)) {
                float dt = ere::application::get_application()->get_delta_time();
                glm::vec3 front = camera.m_front;

                float delta = glm::length(event.get_delta_pos());
                delta *= glm::sign(event.get_delta_pos().y + event.get_delta_pos().x);

                event.get_registry().get<transform>(camera_entity).m_position.x += front.x * delta * dt * move_speed;
                event.get_registry().get<transform>(camera_entity).m_position.y += front.y * delta * dt * move_speed;
                event.get_registry().get<transform>(camera_entity).m_position.z += front.z * delta * dt * move_speed;
            }
        } else {
            // turn
            if (event.get_registry().any_of<ege::transform>(camera_entity)) {
                float dt = ere::application::get_application()->get_delta_time();

                event.get_registry().get<transform>(camera_entity).m_rotation.x += event.get_delta_pos().y * dt * orbit_speed;
                event.get_registry().get<transform>(camera_entity).m_rotation.y -= event.get_delta_pos().x * dt * orbit_speed;
            }
        }
    }

    return false;
}

void scene_camera_system::update_camera(entt::registry& t_registery, entt::entity &camera_entity, scene_camera& t_camera) {
    glm::vec3 pos = {0.0f, 0.0f, 0.0f};
    float yaw = -90.0f;
    float pitch = 0.0f;
    glm::vec3 world_up = {0.0f, -1.0f, 0.0f};

    if (t_registery.any_of<ege::transform>(camera_entity)) {
        auto& transform = t_registery.get<ege::transform>(camera_entity);

        pos = transform.m_position;
        yaw = transform.m_rotation.y;
        pitch = transform.m_rotation.x;
    }

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    t_camera.m_front = glm::normalize(front);
    t_camera.m_right = glm::normalize(glm::cross(t_camera.m_front, world_up));
    t_camera.m_up = glm::normalize(glm::cross(t_camera.m_right, t_camera.m_front));

    t_camera.m_camera->m_view_matrix = glm::lookAt(pos, pos + t_camera.m_front, t_camera.m_up);
    t_camera.m_camera->m_projection_matrix = glm::perspective(glm::radians(t_camera.m_fov), t_camera.m_aspect_ratio, t_camera.m_near, t_camera.m_far);
}


entt::entity scene_camera_system::get_camera_entity(entt::registry& t_registery) {
    auto view = t_registery.view<scene_camera>();

    for (auto entity : view) {
        auto& camera = view.get<scene_camera>(entity);

        if (camera.m_main_camera) {
            return entity;
        }
    }

    return create_camera(t_registery);
}

scene_camera& scene_camera_system::get_camera(entt::registry& t_registery) {
    auto view = t_registery.view<scene_camera>();

    for (auto entity : view) {
        auto& camera = view.get<scene_camera>(entity);

        if (camera.m_main_camera) {
            return camera;
        }
    }

    auto entity = create_camera(t_registery);
    return t_registery.get<scene_camera>(entity);
}

entt::entity scene_camera_system::create_camera(entt::registry& t_registery) {
    auto entity = t_registery.create();
    t_registery.emplace<scene_camera>(entity);
    t_registery.emplace<ege::transform>(entity);
    t_registery.emplace<ege::UUID>(entity, UUID_generator::generate());
    t_registery.emplace<ege::tag>(entity, tag_generator::generate("Scene Camera"));
    t_registery.emplace<ege::skybox>(entity, skybox_generator::generate({
        "res/skybox/default_skybox/right.jpg",
        "res/skybox/default_skybox/left.jpg",
        "res/skybox/default_skybox/top.jpg",
        "res/skybox/default_skybox/bottom.jpg",
        "res/skybox/default_skybox/front.jpg",
        "res/skybox/default_skybox/back.jpg"
    }));

    t_registery.get<scene_camera>(entity).m_main_camera = true;
    t_registery.get<scene_camera>(entity).m_camera = std::make_shared<scene_camera_api>();
    t_registery.get<scene_camera>(entity).m_fov = 45.0f;

    t_registery.get<transform>(entity).m_position = {0.0f, 10.0f, 20.0f};
    t_registery.get<transform>(entity).m_rotation = {0.0f, -90.0f, 0.0f};

    return entity;
}

}
