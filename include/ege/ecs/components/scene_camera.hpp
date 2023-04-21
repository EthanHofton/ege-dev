#ifndef __EGE_SCENE_CAMERA_HPP__
#define __EGE_SCENE_CAMERA_HPP__

#include <ere/api/camera_api.hpp>

namespace ege {

struct scene_camera {
    // camera options
    glm::vec3 m_position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 m_world_up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 m_front = glm::vec3(0.0f, 0.0f, -1.0f);
    float m_yaw = -90.0f;
    float m_pitch = 0.0f;

    // projection options
    float m_fov = 45.0f;
    float m_aspect_ratio = 1.0f;
    float m_near = 0.1f;
    float m_far = 100.0f;
};

}


#endif
