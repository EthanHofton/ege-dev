#ifndef __EGE_SCENE_VIEW_SYSTEM_HPP__
#define __EGE_SCENE_VIEW_SYSTEM_HPP__

#include <ege/ecs/system.hpp>
#include <ere/api/framebuffer_api.hpp>
#include <ere/api/shader_api.hpp>
#include <ere/api/camera_api.hpp>

namespace ege {

class camera_test : public ere::camera_api {
public:
    camera_test () {
        m_projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        m_view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    }

    void set_aspect_ratio(float aspect_ratio) {
        m_projection = glm::perspective(glm::radians(45.0f), aspect_ratio, 0.1f, 100.0f);
    }

    const glm::mat4& get_projection_matrix() override {
        return m_projection;
    }

    const glm::mat4& get_view_matrix() override {
        return m_view;
    }

    glm::vec3 get_position() const override {
        return glm::vec3(0.0f, 0.0f, 0.0f);
    }

private:

    glm::mat4 m_projection;
    glm::mat4 m_view;
};

class scene_view_system : public system {
public:

    scene_view_system();

    virtual bool on_pre_draw(pre_draw_event& event) override;
    virtual bool on_draw(draw_event& event) override;
    virtual bool on_post_draw(post_draw_event& event) override;

    virtual bool on_gui_draw(gui_draw_event& event) override;

private:

    ere::ref<ere::framebuffer_api> m_drawbuffer;
    ere::ref<ere::shader_api> m_shader;
    ere::ref<camera_test> m_camera;

};

}

#endif // __EGE_SCENE_VIEW_SYSTEM_HPP__
