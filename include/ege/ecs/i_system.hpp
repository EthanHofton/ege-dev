#ifndef __EGE_I_SYSTEM_HPP__
#define __EGE_I_SYSTEM_HPP__

#include <ege/events/event.hpp>

namespace ege {

class i_system {
public:

    virtual void on_event(ege_event& t_e) = 0;

};

}


#endif // __EGE_I_SYSTEM_HPP__
