#include <ege/ecs/scene.hpp>
#include <ege/ecs/components/transform.hpp>
#include <ege/ecs/components/mesh.hpp>
#include <ege/ecs/components/renderer.hpp>
#include <ege/ecs/components/scene_camera.hpp>
#include <ege/ecs/systems/scene_system.hpp>
#include <ege/ecs/components/skybox.hpp>
#include <ege/ecs/components/material.hpp>
#include <ege/ecs/components/point_light.hpp>
#include <ege/ecs/components/spot_light.hpp>
#include <ege/ecs/components/directional_light.hpp>
#include <ege/ecs/components/area_light.hpp>

#include <ere/mappings/mouse_map.hpp>

#include <imgui.h>

namespace ege {

scene::scene() {
    static int created_scenes = 0;
    m_scene_name = "Scene " + std::to_string(created_scenes++);

    ere::ref<inspector_system> inspector_s = system_manager::get<inspector_system>();
    inspector_s->register_component<tag>("Tag", false, false);
    inspector_s->register_component<transform>("Transform");
    inspector_s->register_component<mesh>("Mesh");
    inspector_s->register_component<renderer>("Renderer");
    inspector_s->register_component<UUID>("UUID");
    inspector_s->register_component<skybox>("Skybox");
    inspector_s->register_component<material>("Material");

    inspector_s->register_component<point_light>("Point Light");
    inspector_s->register_component<spot_light>("Spot Light");
    inspector_s->register_component<directional_light>("Directional Light");
    inspector_s->register_component<area_light>("Area Light");
};

entt::entity scene::create_entity() {
    auto entity = m_registry.create();
    m_registry.emplace<UUID>(entity, UUID_generator::generate());
    m_registry.emplace<tag>(entity, "Entity " + std::to_string((int)entity));
    return entity;
}

entt::entity scene::create_entity(const std::string& name) {
    auto entity = m_registry.create();
    m_registry.emplace<UUID>(entity, UUID_generator::generate());
    m_registry.emplace<tag>(entity, tag_generator::generate(name));
    return entity;
}

void scene::on_update(ere::update_event& event) {
    ege::update_event e_update(m_registry);
    ege::pre_draw_event e_pre_draw(m_registry);
    ege::draw_event e_draw(m_registry);
    ege::post_draw_event e_post_draw(m_registry);

    system_manager::on_event(e_update);
    system_manager::on_event(e_pre_draw);
    system_manager::on_event(e_draw);
    system_manager::on_event(e_post_draw);
}

void scene::on_imgui_update(ere::imgui_update_event& event) {
    ege::gui_draw_event e(m_registry);

    system_manager::on_event(e);
}

void scene::on_key_pressed(ere::key_pressed_event& event) {
    ege::key_pressed_event e(m_registry, event.get_key_code(), event.get_repete_count());

    system_manager::on_event(e);
}

void scene::on_key_released(ere::key_released_event& event) {
    ege::key_released_event e(m_registry, event.get_key_code());

    system_manager::on_event(e);
}

void scene::on_key_typed(ere::key_typed_event& event) {
    ege::key_typed_event e(m_registry, event.get_key_code());

    system_manager::on_event(e);
}

void scene::on_mouse_button_pressed(ere::mouse_button_pressed_event& event) {
    ege::mouse_button_pressed_event e(m_registry, event.get_mouse_button());

    system_manager::on_event(e);
}

void scene::on_mouse_button_released(ere::mouse_button_released_event& event) {
    ege::mouse_button_released_event e(m_registry, event.get_mouse_button());

    system_manager::on_event(e);
}

void scene::on_mouse_moved(ere::mouse_moved_event& event) {
    ege::mouse_moved_event e(m_registry, {event.get_pos().x, event.get_pos().y}, {event.get_delta_pos().x, event.get_delta_pos().y});

    system_manager::on_event(e);
}

void scene::on_mouse_scrolled(ere::mouse_scrolled_event& event) {
    ege::mouse_scrolled_event e(m_registry, {event.get_offset().x, event.get_offset().y});

    system_manager::on_event(e);
}

}
