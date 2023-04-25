#ifndef __EGE_SCENE_CAMERA_HPP__
#define __EGE_SCENE_CAMERA_HPP__

#include <ere/core/core.hpp>
#include <ere/api/camera_api.hpp>

namespace ege {

class scene_camera_api : public ere::camera_api {
public:

    inline const glm::mat4& get_view_matrix() override { return m_view_matrix; }
    inline const glm::mat4& get_projection_matrix() override { return m_projection_matrix; }
    inline glm::vec3 get_position() const override { return glm::vec3(m_view_matrix[3]); }

    // matricies
    glm::mat4 m_view_matrix = glm::mat4(1.0f);
    glm::mat4 m_projection_matrix = glm::mat4(1.0f);
};

struct scene_camera {
    ere::ref<scene_camera_api> m_camera = nullptr;

    bool m_main_camera = false;

    // projection options
    float m_fov = 45.0f;
    float m_aspect_ratio = 1.0f;
    float m_near = 0.1f;
    float m_far = 100.0f;

    // calculated
    glm::vec3 m_up;
    glm::vec3 m_front = {0, 0, -1};
    glm::vec3 m_right;
};

}


#endif
