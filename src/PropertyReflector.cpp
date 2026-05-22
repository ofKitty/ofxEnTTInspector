#include "PropertyReflector.h"
#include "PropertyValue.h"
#include "ComponentInspector.h"
#include "ofJson.h"
#include "inspectors/inspectors.h"
#include <ofxEnTTKit.h>
#include "components/filter_components.h"
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

    REFLECT_CTX(ecs::node_component)
    REFLECT(ecs::render_component)
    REFLECT(ecs::camera_component)
    REFLECT(ecs::fbo_component)
    REFLECT(ecs::tag_component)
    REFLECT(ecs::selectable_component)
    REFLECT(ecs::mesh_component)
    REFLECT_CTX(ecs::model_component)
    REFLECT_CTX(ecs::image_component)
    REFLECT_CTX(ecs::video_component)

    REFLECT(ecs::light_component)
    REFLECT(ecs::material_component)
    REFLECT(ecs::shader_component)
    REFLECT(ecs::primitive_component)
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

    REFLECT(ecs::shape2d_component)

    REFLECT(ecs::grid_helper_component)
    REFLECT(ecs::gizmo_component)
    REFLECT(ecs::bounding_box_component)
    REFLECT(ecs::mask_component)
    REFLECT(ecs::rigidbody_component)

    REFLECT(ecs::serial_component)
    REFLECT(ecs::osc_component)
    REFLECT(ecs::audio_source_component)
    REFLECT(ecs::midi_source_component)
    REFLECT(ecs::canvas_effects_component)

    REFLECT(ecs::uv_component)
    REFLECT(ecs::uv_sample_component)

    REFLECT(ecs::swatch_library_component)
    REFLECT(ecs::color_gradient_component)

    REFLECT(ecs::tint_filter_component)
    REFLECT(ecs::invert_filter_component)
    REFLECT(ecs::mirror_filter_component)
    REFLECT(ecs::color_adjust_component)
    REFLECT(ecs::blur_filter_component)
    REFLECT(ecs::dither_filter_component)
    REFLECT(ecs::rotate_filter_component)
    REFLECT(ecs::threshold_filter_component)
    REFLECT(ecs::posterize_filter_component)
    REFLECT(ecs::noise_filter_component)
    REFLECT(ecs::vignette_filter_component)
    REFLECT(ecs::chromatic_aberration_component)
    REFLECT(ecs::pixelate_filter_component)
    REFLECT(ecs::mesh_filter_component)
    REFLECT(ecs::rings_filter_component)
    REFLECT(ecs::line_scan_filter_component)
    REFLECT(ecs::ascii_filter_component)

    REFLECT(ecs::gradient_generator_component)
    REFLECT(ecs::dots_generator_component)
    REFLECT(ecs::stripes_generator_component)
    REFLECT(ecs::checkerboard_generator_component)
    REFLECT(ecs::noise_generator_component)

    REFLECT(ecs::eased_pulse_component)

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

void pullNodeTransformCaches(entt::registry& reg) {
    for (auto [e, nc] : reg.view<ecs::node_component>().each()) {
        nc.cachedPosition = nc.node.getPosition();
        nc.cachedScale    = nc.node.getScale();
    }
}

void pushNodeTransformCaches(entt::registry& reg) {
    for (auto [e, nc] : reg.view<ecs::node_component>().each()) {
        if (nc.cachedPosition != nc.node.getPosition()) nc.node.setPosition(nc.cachedPosition);
        if (nc.cachedScale    != nc.node.getScale())    nc.node.setScale(nc.cachedScale);
    }
}

} // namespace inspector
