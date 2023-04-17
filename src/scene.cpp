#include <ege/ecs/scene.hpp>
#include <ege/ecs/entity.hpp>
#include <ege/ecs/components/UUID.hpp>
#include <ege/ecs/components/tag.hpp>

namespace ege {

ege::entity scene::create_entity() {
    static int created_entities = 0;

    ege::entity e = ege::entity(m_registry.create(), shared_from_this());
    e.add_component<ege::UUID>(ege::UUID_generator::generate());
    e.add_component<ege::tag>(ege::tag_generator::generate("Entity " + std::to_string(created_entities++)));

    return e;
}

void scene::destroy_entity(ege::entity t_entity) {
    m_registry.destroy(t_entity.m_entt_entity);
}

std::vector<ege::entity> scene::get_entities() {
    std::vector<ege::entity> entities;
    m_registry.each([&](auto entity) {
        entities.push_back({entity, shared_from_this()});
    });
    return entities;
}

}
