#ifndef __EGE_UUID_HPP__
#define __EGE_UUID_HPP__

#include <string>
#include <util/algo.hpp>
#include <ege/ecs/systems/inspector.hpp>

namespace ege {

struct UUID {
    std::string m_uuid;
};

struct UUID_generator {
    static UUID generate() {
        UUID u;
        u.m_uuid = util::generate_uuid_v4();
        return u;
    }
};

template<>
inline void inspector_system::component_editor_widget<UUID>(entt::registry& t_registery, entt::entity t_entity) {
    auto& uuid = t_registery.get<UUID>(t_entity);
    ImGui::Text("UUID: %s", uuid.m_uuid.c_str());
}

}

#endif
