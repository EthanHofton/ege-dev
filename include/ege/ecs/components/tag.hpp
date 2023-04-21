#ifndef __EGE_TAG_HPP__
#define __EGE_TAG_HPP__

#include <string>
#include <ege/ecs/systems/inspector_system.hpp>
#include <misc/cpp/imgui_stdlib.h>

namespace ege {

struct tag {
    std::string m_tag;
};

struct tag_generator {
    static tag generate(std::string t_tag) {
        tag u;
        u.m_tag = t_tag;
        return u;
    }
};

template<>
inline void inspector_system::component_editor_widget<ege::tag>(entt::registry& t_registery, entt::entity t_entity) {
    auto& tag = t_registery.get<ege::tag>(t_entity);
    ImGui::InputText("Tag##change_val", &tag.m_tag);
}

}

#endif
