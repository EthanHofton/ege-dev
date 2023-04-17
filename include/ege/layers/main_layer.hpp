#ifndef __EGE_MAIN_LAYER_HPP__
#define __EGE_MAIN_LAYER_HPP__

#include <ege/core/logger.hpp>
#include <ere/core/layer.hpp>
#include <imgui.h>
#include <ege/ecs/scene.hpp>
#include <ere/core/core.hpp>
#include <ege/ecs/entity.hpp>

namespace ege {

class main_layer : public ere::layer {
public:

    bool on_attach(ere::attach_event& e) override;
    bool on_detach(ere::detach_event& e) override;
    bool on_update(ere::update_event& e) override;
    bool on_imgui_update(ere::imgui_update_event& e) override;

private:

    void setup_dockspace();
    void draw_menu_bar();
    void draw_inspector();
    void draw_scene_hierarchy();
    void draw_scene_viewport();
    void draw_game_viewport();
    void draw_project_explorer();

    std::string m_inspector_name = "Inspector";
    std::string m_scene_hierarchy_name = "Scene Hierarchy";
    std::string m_scene_viewport_name = "Scene Viewport";
    std::string m_game_viewport_name = "Game Viewport";
    std::string m_project_explorer_name = "Project Explorer";

    std::vector<ere::ref<scene>> m_scenes;
    ere::ref<scene> m_active_scene;
    entity m_selected_entity;
};

}

#endif
