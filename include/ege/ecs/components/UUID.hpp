#ifndef __EGE_UUID_HPP__
#define __EGE_UUID_HPP__

#include <string>
#include <util/algo.hpp>

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

}

#endif
