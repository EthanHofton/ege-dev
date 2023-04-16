#ifndef __EGE_TAG_HPP__
#define __EGE_TAG_HPP__

#include <string>

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

}

#endif
