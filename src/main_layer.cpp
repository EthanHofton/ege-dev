#include <ege/layers/main_layer.hpp>
#include <imgui_internal.h>

namespace ege {

bool main_layer::on_attach(ere::attach_event& e) {
    EGE_INFO("Main layer attached");

    m_scenes.push_back(ere::createRef<scene>());

    m_scenes[0]->create_entity();
    m_scenes[0]->create_entity();
    m_scenes[0]->create_entity();
    m_scenes[0]->create_entity();

    m_scenes.push_back(ere::createRef<scene>());

    m_scenes[1]->create_entity();

    m_active_scene = m_scenes[0];

    return false;
}

bool main_layer::on_detach(ere::detach_event& e) {
    EGE_INFO("Main layer detached");
    return false;
}

bool main_layer::on_update(ere::update_event& e) {
    m_active_scene->on_update(e);

    return false;
}

bool main_layer::on_imgui_update(ere::imgui_update_event& e) {

    setup_dockspace();

    draw_inspector();
    draw_scene_hierarchy();
    draw_scene_viewport();
    draw_game_viewport();
    draw_project_explorer();

    m_active_scene->on_imgui_update(e);

    return false;
}

void main_layer::draw_menu_bar() {
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            ImGui::MenuItem("New");
            ImGui::MenuItem("Open");
            ImGui::MenuItem("Save");
            ImGui::MenuItem("Save As");
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Edit")) {
            ImGui::MenuItem("Undo");
            ImGui::MenuItem("Redo");
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("View")) {
            ImGui::MenuItem("Inspector");
            ImGui::MenuItem("Scene Hierarchy");
            ImGui::MenuItem("Scene Viewport");
            ImGui::MenuItem("Game Viewport");
            ImGui::MenuItem("Project Explorer");
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Game")) {
            ImGui::MenuItem("Play");
            ImGui::MenuItem("Stop");
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }
}

void main_layer::draw_inspector() {
    ImGui::Begin(m_inspector_name.c_str());

    m_active_scene->on_inspector_draw();

    ImGui::End();

}

void main_layer::draw_scene_hierarchy() {
    ImGui::Begin(m_scene_hierarchy_name.c_str());

    for (auto& scene : m_scenes) {
        scene->on_hierarchy_draw(m_active_scene);
    }

    ImGui::End();
}

void main_layer::draw_scene_viewport() {
    ImGui::Begin(m_scene_viewport_name.c_str());

    m_active_scene->on_scene_viewport_draw();

    ImGui::End();

}

void main_layer::draw_game_viewport() {
    ImGui::Begin(m_game_viewport_name.c_str());

    m_active_scene->simulate();

    ImGui::End();

}

void main_layer::draw_project_explorer() {
    ImGui::Begin(m_project_explorer_name.c_str());
    ImGui::End();

}

void main_layer::setup_dockspace() {
    static bool dockspace_open = true;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace", &dockspace_open, window_flags);
    ImGui::PopStyleVar();

    ImGui::PopStyleVar(2);

    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
        ImGuiID dockspace_id = ImGui::GetID("MainDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    draw_menu_bar();

    ImGui::End();
}


}
