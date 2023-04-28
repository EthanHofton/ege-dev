#include <ege/ecs/systems/scene_system.hpp>
#include <ege/ecs/system_manager.hpp>
#include <ege/ecs/scene.hpp>
#include <ege/ecs/components/mesh.hpp>
#include <ege/ecs/components/renderer.hpp>
#include <ege/ecs/components/transform.hpp>
#include <ege/ecs/components/material.hpp>
#include <ege/ecs/components/spot_light.hpp>
#include <ege/ecs/components/point_light.hpp>
#include <ege/ecs/components/directional_light.hpp>
#include <ege/ecs/components/area_light.hpp>

namespace ege {

bool scene_system::on_gui_draw(gui_draw_event& event) {
    ImGui::Begin("Scene Hierarchy");

    if (ImGui::BeginPopupContextWindow()) {
        if (ImGui::MenuItem("New Entity")) {
            auto e = m_selected_scene->create_entity();
            m_selected_entity = e;
        }

        if (ImGui::MenuItem("New Cube")) {
            auto e = m_selected_scene->create_entity("Cube");
            m_selected_entity = e;
            m_selected_scene->get_registry().emplace<mesh>(e, mesh_generator::generate_cube_mesh({1,1,1}));
            m_selected_scene->get_registry().emplace<renderer>(e);
            m_selected_scene->get_registry().emplace<transform>(e);
            m_selected_scene->get_registry().emplace<material>(e, material_generator::generate_default_material());
        }

        if (ImGui::MenuItem("New Sphere")) {
            auto e = m_selected_scene->create_entity("Sphere");
            m_selected_entity = e;
            m_selected_scene->get_registry().emplace<mesh>(e, mesh_generator::generate_sphere_mesh(1));
            m_selected_scene->get_registry().emplace<renderer>(e);
            m_selected_scene->get_registry().emplace<transform>(e);
            m_selected_scene->get_registry().emplace<material>(e, material_generator::generate_default_material());
        }

        if (ImGui::MenuItem("New Plane")) {
            auto e = m_selected_scene->create_entity("Plane");
            m_selected_entity = e;
            m_selected_scene->get_registry().emplace<mesh>(e, mesh_generator::generate_plane_mesh({1,1}));
            m_selected_scene->get_registry().emplace<renderer>(e);
            m_selected_scene->get_registry().emplace<transform>(e);
            m_selected_scene->get_registry().emplace<material>(e, material_generator::generate_default_material());
        }

        if (ImGui::MenuItem("New Point Light")) {
            auto e = m_selected_scene->create_entity("Point Light");
            m_selected_entity = e;
            m_selected_scene->get_registry().emplace<point_light>(e);
            m_selected_scene->get_registry().emplace<transform>(e);
        }

        if (ImGui::MenuItem("New Spot Light")) {
            auto e = m_selected_scene->create_entity("Spot Light");
            m_selected_entity = e;
            m_selected_scene->get_registry().emplace<spot_light>(e);
            m_selected_scene->get_registry().emplace<transform>(e);
        }

        if (ImGui::MenuItem("New Directional Light")) {
            auto e = m_selected_scene->create_entity("Directional Light");
            m_selected_entity = e;
            m_selected_scene->get_registry().emplace<directional_light>(e);
            m_selected_scene->get_registry().emplace<transform>(e);
        }

        if (ImGui::MenuItem("New Area Light")) {
            auto e = m_selected_scene->create_entity("Area Light");
            m_selected_entity = e;
            m_selected_scene->get_registry().emplace<area_light>(e);
            m_selected_scene->get_registry().emplace<transform>(e);
        }

        ImGui::EndPopup();
    }

    for (auto& entity : event.get_registry().view<tag>()) {
        auto& tag = event.get_registry().get<ege::tag>(entity);


        // list all entities in scene
        ImGui::PushID((int)entity);
        if (ImGui::Selectable(tag.m_tag.c_str(), m_selected_entity == entity)) {
            if (m_selected_entity == entity) {
                m_selected_entity = entt::null;
            } else {
                m_selected_entity = entity;
            }
        }
        ImGui::PopID();
    }

    ImGui::End();

    ImGui::Begin("Scene Selector");

    if (ImGui::BeginPopupContextWindow()) {
        if (ImGui::MenuItem("New Scene")) {
            auto s = add_scene("New Scene");
            m_selected_scene = s;
            m_selected_entity = entt::null;
        }

        ImGui::EndPopup();
    }

    int i = 0;
    for (auto& s : m_scenes) {
        ImGui::PushID(i++);
        if (ImGui::Selectable(s->get_scene_name().c_str(), m_selected_scene == s)) {
            m_selected_scene = s;
            m_selected_entity = entt::null;
        }
        ImGui::PopID();
    }

    ImGui::End();

    return false;
}

void scene_system::select_scene(const std::string& t_name) {
    ere::ref<scene> s = get_scene(t_name);

    if (s) {
        m_selected_scene = s;
    }
}

ere::ref<scene> scene_system::add_scene(const std::string& t_name) {
    ere::ref<scene> s = ere::createRef<scene>();
    s->set_scene_name(t_name);
    m_scenes.push_back(s);

    if (m_selected_scene == nullptr) {
        m_selected_scene = s;
    }

    return s;
}

void scene_system::add_scene(ere::ref<scene> t_scene) {
    m_scenes.push_back(t_scene);

    if (m_selected_scene == nullptr) {
        m_selected_scene = t_scene;
    }
}

ere::ref<scene> scene_system::get_scene(const std::string& t_name) {
    for (auto& s : m_scenes) {
        if (s->get_scene_name() == t_name) {
            return s;
        }
    }

    return nullptr;
}

void scene_system::remove_scene(const std::string& t_name) {
    ere::ref<scene> s = get_scene(t_name);

    if (s) {
        remove_scene(s);
    }
}
void scene_system::remove_scene(ere::ref<scene> t_scene) {
    m_scenes.erase(std::remove(m_scenes.begin(), m_scenes.end(), t_scene), m_scenes.end());
}

void scene_system::on_update(ere::update_event& event) {
    if (m_selected_scene) {
        m_selected_scene->on_update(event);
    }
}

void scene_system::on_imgui_update(ere::imgui_update_event& event) {
    if (m_selected_scene) {
        m_selected_scene->on_imgui_update(event);
    }
}

void scene_system::on_key_pressed(ere::key_pressed_event& event) {
    if (m_selected_scene) {
        m_selected_scene->on_key_pressed(event);
    }
}

void scene_system::on_key_released(ere::key_released_event& event) {
    if (m_selected_scene) {
        m_selected_scene->on_key_released(event);
    }
}

void scene_system::on_key_typed(ere::key_typed_event& event) {
    if (m_selected_scene) {
        m_selected_scene->on_key_typed(event);
    }
}

void scene_system::on_mouse_button_pressed(ere::mouse_button_pressed_event& event) {
    if (m_selected_scene) {
        m_selected_scene->on_mouse_button_pressed(event);
    }
}

void scene_system::on_mouse_button_released(ere::mouse_button_released_event& event) {
    if (m_selected_scene) {
        m_selected_scene->on_mouse_button_released(event);
    }
}

void scene_system::on_mouse_moved(ere::mouse_moved_event& event) {
    if (m_selected_scene) {
        m_selected_scene->on_mouse_moved(event);
    }
}

void scene_system::on_mouse_scrolled(ere::mouse_scrolled_event& event) {
    if (m_selected_scene) {
        m_selected_scene->on_mouse_scrolled(event);
    }
}


}
