#ifndef __EGE_MATERIAL_SYSTEM_HPP__
#define __EGE_MATERIAL_SYSTEM_HPP__

#include <ege/ecs/system.hpp>

namespace ege {

class material_system : public system {
public:

    bool on_pre_draw(pre_draw_event& event) override;

};

}

#endif // __EGE_MATERIAL_SYSTEM_HPP__
