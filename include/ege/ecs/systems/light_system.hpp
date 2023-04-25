#ifndef __EGE_LIGHT_SYSTEM_HPP__
#define __EGE_LIGHT_SYSTEM_HPP__

#include <ege/ecs/system.hpp>

namespace ege {

class light_system : public system {
public:

    bool on_pre_draw(pre_draw_event& event) override;

};

}

#endif // __EGE_LIGHT_SYSTEM_HPP__
