#include <ege/ecs/scene.hpp>
#include <imgui.h>

namespace ege {

scene::scene() {
    static int created_scenes = 0;
    m_scene_name = "Scene " + std::to_string(created_scenes++);

    m_debug_system = std::make_shared<debug_system>();
    m_debug_system->register_component<tag>("Tag");
    m_debug_system->register_component<UUID>("UUID");

    add_system(m_debug_system);
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
    for (auto& system : m_systems) {
        system->on_update(event, m_registry);
    }
}

void scene::on_imgui_update(ere::imgui_update_event& event) {
    for (auto& system : m_systems) {
        system->on_imgui_update(event, m_registry);
    }
}

void scene::on_attach(ere::attach_event& event) {
    for (auto& system : m_systems) {
        system->on_attach(event, m_registry);
    }
}

void scene::on_detach(ere::detach_event& event) {
    for (auto& system : m_systems) {
        system->on_detach(event, m_registry);
    }
}

void scene::on_key_pressed(ere::key_pressed_event& event) {
    for (auto& system : m_systems) {
        system->on_key_pressed(event, m_registry);
    }
}

void scene::on_key_released(ere::key_released_event& event) {
    for (auto& system : m_systems) {
        system->on_key_released(event, m_registry);
    }
}

void scene::on_key_typed(ere::key_typed_event& event) {
    for (auto& system : m_systems) {
        system->on_key_typed(event, m_registry);
    }
}

void scene::on_mouse_button_pressed(ere::mouse_button_pressed_event& event) {
    for (auto& system : m_systems) {
        system->on_mouse_button_pressed(event, m_registry);
    }
}

void scene::on_mouse_button_released(ere::mouse_button_released_event& event) {
    for (auto& system : m_systems) {
        system->on_mouse_button_released(event, m_registry);
    }
}

void scene::on_mouse_moved(ere::mouse_moved_event& event) {
    for (auto& system : m_systems) {
        system->on_mouse_moved(event, m_registry);
    }
}

void scene::on_mouse_scrolled(ere::mouse_scrolled_event& event) {
    for (auto& system : m_systems) {
        system->on_mouse_scrolled(event, m_registry);
    }
}

void scene::on_window_resized(ere::window_resized_event& event) {
    for (auto& system : m_systems) {
        system->on_window_resized(event, m_registry);
    }
}

void scene::on_window_moved(ere::window_moved_event& event) {
    for (auto& system : m_systems) {
        system->on_window_moved(event, m_registry);
    }
}

void scene::on_window_minimized(ere::window_minimized_event& event) {
    for (auto& system : m_systems) {
        system->on_window_minimized(event, m_registry);
    }
}

void scene::on_window_maximized(ere::window_maximized_event& event) {
    for (auto& system : m_systems) {
        system->on_window_maximized(event, m_registry);
    }
}

void scene::on_window_focus(ere::window_focus_event& event) {
    for (auto& system : m_systems) {
        system->on_window_focus(event, m_registry);
    }
}

void scene::on_window_closed(ere::window_closed_event& event) {
    for (auto& system : m_systems) {
        system->on_window_closed(event, m_registry);
    }
}

void scene::on_hierarchy_draw(ere::ref<scene>& t_active_scene) {
    ere::ref<scene> t = shared_from_this();
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
    if (t_active_scene == t) {
        flags |= ImGuiTreeNodeFlags_Selected;

    } else {
        m_selected_entity = entt::null;
    }

    if (ImGui::TreeNodeEx(m_scene_name.c_str(), flags)) {
        
        if (t != t_active_scene) {
            if (ImGui::IsItemClicked()) {
                t_active_scene = t;
            }
        }

        for (auto& entity : m_registry.view<tag>()) {
            auto& tag = m_registry.get<ege::tag>(entity);

            if (ImGui::Selectable(tag.m_tag.c_str(), m_selected_entity == entity)) {
                t_active_scene = t;
                m_selected_entity = entity;
            }
        }

        ImGui::TreePop();
    }
}

void scene::on_inspector_draw() {
    for (auto& system : m_systems) {
        system->on_inspector_draw(m_registry, m_selected_entity);
    }
}

void scene::on_scene_viewport_draw() {

}

void scene::simulate() {

}


}
