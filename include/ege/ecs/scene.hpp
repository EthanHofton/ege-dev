#ifndef __EGE_SCENE_HPP__
#define __EGE_SCENE_HPP__

#include <entt/entt.hpp>
#include <memory>

namespace ege {

class entity;
class scene : public std::enable_shared_from_this<scene> {
public:
    friend class entity;

    scene() {
        static int created_scenes = 0;
        m_scene_name = "Scene " + std::to_string(created_scenes++);
    };

    ege::entity create_entity();
    void destroy_entity(ege::entity t_entity);

    std::vector<ege::entity> get_entities();

    std::string get_name() { return m_scene_name; }
    void set_name(const std::string& t_name) { m_scene_name = t_name; }


private:

    entt::registry m_registry;
    std::string m_scene_name;

};

}

#endif // __EGE_SCENE_HPP__
