#ifndef __EGE_SYSTEM_MANAGER_HPP__
#define __EGE_SYSTEM_MANAGER_HPP__

#include <ere/core/core.hpp>
#include <typeindex>

#include "i_system.hpp"
#include <ege/events/app_events.hpp>

namespace ege {

class system_manager {
public:

    ~system_manager() {
        // call on event with detach event
        for (auto& s : m_systems) {
            if (s == nullptr) {
                continue;
            }
            ege::detach_event e;
            s->on_event(e);
        }

        m_systems.clear();
    }

    template<typename T>
    inline static ere::ref<T> get() {
        for (auto& s : m_systems) {
            if (s == nullptr) {
                continue;
            }
            i_system& s_ref = *s;
            if (typeid(s_ref) == typeid(T)) {
                return std::dynamic_pointer_cast<T>(s);
            }
        }

        // if not found, create it
        ere::ref<T> new_system = ere::createRef<T>();
        m_systems.push_back(new_system);
        ege::attach_event e;
        new_system->on_event(e);
        return new_system;
    }

    template<typename T>
    inline static void remove() {
        auto it = std::remove_if(m_systems.begin(), m_systems.end(),
            [](const auto& pair) { return pair.first == std::type_index(typeid(T)); });
        ege::detach_event e;
        *it->on_event(e);
        m_systems.erase(it, m_systems.end());
    }

    inline static void remove(ere::ref<i_system> t_system) {
        auto it = std::find(m_systems.begin(), m_systems.end(), t_system);
        if (it != m_systems.end()) {
            ege::detach_event e;
            t_system->on_event(e);
            m_systems.erase(it);
        }
    }

    inline static void on_event(ege_event& t_e) {
        for (auto& t_system : m_systems) {
            t_system->on_event(t_e);
        }
    }

private:

    static std::vector<ere::ref<i_system>> m_systems;
};

}

#endif // __EGE_SYSTEM_MANAGER_HPP__
