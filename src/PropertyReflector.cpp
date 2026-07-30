#include "PropertyReflector.h"
#include "PropertyValue.h"
#include "ComponentInspector.h"
#include "ofJson.h"
#include "inspectors/inspectors.h"
#include <ofxEnTTKit_all.h>
#include "components/eased_pulse_component.h"

namespace inspector {

// ─── PropertyReflector::serialize ────────────────────────────────────────────

ofJson PropertyReflector::serialize() const
{
    ofJson j = ofJson::object();
    for (const auto& prop : m_props) {
        if (prop.type == PinDataType::Trigger || prop.type == PinDataType::Any) continue;
        j[prop.name] = PropertyValue::from(prop).toJson();
    }
    return j;
}

// ─── PropertyReflector::deserialize ──────────────────────────────────────────

bool PropertyReflector::deserialize(const ofJson& j)
{
    bool anyChanged = false;
    for (auto& prop : m_props) {
        if (!j.contains(prop.name)) continue;
        PropertyValue::fromJson(j[prop.name], prop.type).apply(prop);
        anyChanged = true;
    }
    return anyChanged;
}

#define REFLECT(T) if (reg.any_of<T>(e)) { inspector::registerProperties(reg.get<T>(e), col); }
#define REFLECT_CTX(T) if (reg.any_of<T>(e)) { inspector::registerProperties(reg.get<T>(e), col, reg, e); }

std::vector<ReflectedProperty> getEntityProperties(entt::registry& reg, entt::entity e) {
    ComponentInspector col("_reflect");
    if (!reg.valid(e)) return col.getReflectedProperties();

    REFLECT_CTX(ecs::named_entity)
    REFLECT(ecs::render_component)
    REFLECT(ecs::camera_component)
    REFLECT(ecs::fbo_component)
    REFLECT(ecs::tag_component)
    REFLECT(ecs::selectable_component)
    REFLECT(ecs::skew_component)
    REFLECT(ecs::mesh_component)
    REFLECT_CTX(ecs::model_component)
    REFLECT_CTX(ecs::image_component)
    REFLECT_CTX(ecs::video_component)

    REFLECT(ecs::light_component)
    REFLECT(ecs::projector_component)
    REFLECT(ecs::material_component)
    REFLECT(ecs::shader_component)
    REFLECT(ecs::texture_component)
    REFLECT(ecs::cubemap_component)
    REFLECT(ecs::billboard_component)
    REFLECT(ecs::trail_component)
    REFLECT(ecs::tube_component)
    REFLECT(ecs::surface_component)
    REFLECT(ecs::instanced_mesh_component)
    REFLECT(ecs::outline_component)
    REFLECT(ecs::shadow_component)
    REFLECT(ecs::glow_component)

    REFLECT(ecs::tween_component)
    REFLECT(ecs::particle_emitter_component)
    REFLECT(ecs::postfx_component)

    REFLECT(ecs::path_component)
    REFLECT(ecs::corner_radius_component)
    REFLECT(ecs::curve_resolution_component)
    REFLECT(ecs::polyline_component)
    REFLECT(ecs::rectangle_component)
    REFLECT(ecs::circle_component)
    REFLECT(ecs::ellipse_component)
    REFLECT(ecs::line_component)
    REFLECT(ecs::triangle_component)
    REFLECT(ecs::polygon_component)
    REFLECT(ecs::arc_component)
    REFLECT(ecs::bezier_curve_component)
    REFLECT(ecs::spline_component)
    REFLECT(ecs::sprite_component)
    REFLECT(ecs::text_2d_component)
    REFLECT(ecs::star_component)
    REFLECT(ecs::regular_polygon_component)
    REFLECT(ecs::ring_component)
    REFLECT(ecs::cross_component)
    REFLECT(ecs::heart_component)
    REFLECT(ecs::vesica_piscis_component)
    REFLECT(ecs::flower_of_life_component)
    REFLECT(ecs::metatrons_cube_component)
    REFLECT(ecs::soft_mask_component)
    REFLECT(ecs::grid_component)
    REFLECT(ecs::progress_bar_component)
    REFLECT(ecs::arrow_component)

    REFLECT(ecs::grid_helper_component)
    REFLECT(ecs::gizmo_component)
    REFLECT(ecs::bounding_box_component)
    REFLECT(ecs::mask_component)
    REFLECT(ecs::rigidbody_component)

    REFLECT(ecs::audio_source_component)
    REFLECT(ecs::midi_source_component)

    REFLECT(ecs::swatch_library_component)
    REFLECT(ecs::swatch_palette_ref_component)

    REFLECT(ecs::eased_pulse_component)
    REFLECT(ecs::modulator_component)
    REFLECT(ecs::mod_binding_component)

    REFLECT(ecs::clickable_component)
    REFLECT(ecs::click_bind_component)
    REFLECT(ecs::flag_component)

    #undef REFLECT
    #undef REFLECT_CTX

    return col.getReflectedProperties();
}

// ─── ComponentInspector::serialize / deserialize ─────────────────────────────
// Implemented here so PropertyValue.h is only included in one .cpp

ofJson ComponentInspector::serialize() const
{
    ofJson j = ofJson::object();
    for (const auto& prop : m_reflected) {
        if (prop.type == PinDataType::Trigger || prop.type == PinDataType::Any) continue;
        j[prop.name] = PropertyValue::from(prop).toJson();
    }
    return j;
}

bool ComponentInspector::deserialize(const ofJson& j)
{
    bool anyChanged = false;
    for (auto& prop : m_reflected) {
        if (!j.contains(prop.name)) continue;
        PropertyValue::fromJson(j.at(prop.name), prop.type).apply(prop);
        anyChanged = true;
    }
    return anyChanged;
}


} // namespace inspector
