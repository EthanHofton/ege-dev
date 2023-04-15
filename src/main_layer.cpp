#include <ege/layers/main_layer.hpp>

namespace ege {

bool main_layer::on_attach(ere::attach_event& e) {
    EGE_INFO("Main layer attached");
    return false;
}

bool main_layer::on_detach(ere::detach_event& e) {
    EGE_INFO("Main layer detached");
    return false;
}

bool main_layer::on_update(ere::update_event& e) {
    return false;
}

bool main_layer::on_imgui_update(ere::imgui_update_event& e) {
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

    // create a toolbar
    ImGui::SetNextWindowDockID(0, ImGuiCond_Once);
    ImGui::Begin("Toolbar");
    ImGui::Text("Hello, world!");
    ImGui::End();

    return false;
}

}
