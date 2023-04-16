#ifndef __EGE_ENTITY_HPP__
#define __EGE_ENTITY_HPP__

#include <ere/core/core.hpp>
#include <entt/entt.hpp>
#include "scene.hpp"

namespace ege {

class entity {
public:

    friend class scene;

    entity() = default;
    entity(entt::entity t_entt_entity, ere::ref<scene> t_scene)
        : m_entt_entity(t_entt_entity), m_scene(t_scene) {}
    entity(const entity& other) = default;

    template<typename T>
    void add_component(const T& t_component) {
        m_scene->m_registry.emplace<T>(m_entt_entity, t_component);
    }

    template<typename T>
    void remove_component() {
        m_scene->m_registry.remove<T>(m_entt_entity);
    }

    template<typename T>
    T& get_component() {
        return m_scene->m_registry.get<T>(m_entt_entity);
    }

    template<typename T>
    bool has_component() {
        return m_scene->m_registry.any_of<T>(m_entt_entity);
    }

    operator bool() const { return m_entt_entity != entt::null; }
    operator entt::entity() const { return m_entt_entity; }
    bool operator==(const entity& other) const { return m_entt_entity == other.m_entt_entity && m_scene == other.m_scene; }
    bool operator!=(const entity& other) const { return !(*this == other); }


private:

    entt::entity m_entt_entity;
    ere::ref<scene> m_scene;
};

}

#endif // __EGE_ENTITY_HPP__
