#ifndef __EGE_COLORS_HPP__
#define __EGE_COLORS_HPP__

#include <glm/glm.hpp>

namespace ege {


/**
* @brief primary color
*/
inline static constexpr glm::vec4 BASE_COLOR = { 0.169f, 0.169f, 0.169f, 1.0f };

/**
* @brief secondary colors
*/

// light secondary color, #2F2F2F
inline static constexpr glm::vec4 LIGHT_SECONDARY = { 0.184f, 0.184f, 0.184f, 1.0f };
// dark secondary color, #272727
inline static constexpr glm::vec4 DARK_SECONDARY = { 0.157f, 0.157f, 0.157f, 1.0f };

/**
* @brief tertiary colors
*/

// light tertiary color, #343434
inline static constexpr glm::vec4 LIGHT_TERTIARY = { 0.204f, 0.204f, 0.204f, 1.0f };
// dark tertiary color, #232323
inline static constexpr glm::vec4 DARK_TERTIARY = { 0.137f, 0.137f, 0.137f, 1.0f };

}

#endif
