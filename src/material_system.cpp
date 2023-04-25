#include <ege/ecs/systems/material_system.hpp>
#include <ege/ecs/components/material.hpp>
#include <ege/core/logger.hpp>

namespace ege {

bool material_system::on_pre_draw(pre_draw_event& t_e) {
    auto view = t_e.get_registry().view<material>();

    for (auto e : view) {
        auto& mat = view.get<material>(e);

        if (mat.m_active_shader) {
            mat.m_active_shader->set_uniform_3f("u_material.albedo", mat.m_albedo_color);
            mat.m_active_shader->set_uniform_1f("u_material.metallic", mat.m_metallic_value);
            mat.m_active_shader->set_uniform_1f("u_material.roughness", mat.m_roughness_value);
            mat.m_active_shader->set_uniform_1f("u_material.ao", mat.m_ao_value);
            mat.m_active_shader->set_uniform_3f("u_material.emissive", mat.m_emission_color);
        }
    }


    return false;
}

}
