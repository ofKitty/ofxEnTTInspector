#pragma once

// ============================================================================
// Entity Inspector
// Inspect all ECS components on a single entity via ImGui.
// No app framework dependency — pass only the entt::registry.
// ============================================================================

#include <ofxEnTTKit.h>
#include "inspectors/inspectors.h"
#include "ComponentInspector.h"

namespace inspector {

using ecs::node_component;
using ecs::render_component;
using ecs::camera_component;
using ecs::fbo_component;
using ecs::fbo_reference_component;
using ecs::tag_component;
using ecs::selectable_component;
using ecs::mesh_component;
using ecs::model_component;
using ecs::image_component;
using ecs::video_component;
using ecs::resource_component;
using ecs::filepath_component;
using ecs::light_component;
using ecs::material_component;
using ecs::shader_component;
using ecs::primitive_component;
using ecs::texture_component;
using ecs::cubemap_component;
using ecs::billboard_component;
using ecs::trail_component;
using ecs::tube_component;
using ecs::surface_component;
using ecs::instanced_mesh_component;
using ecs::outline_component;
using ecs::shadow_component;
using ecs::glow_component;
using ecs::tween_component;
using ecs::particle_emitter_component;
using ecs::postfx_component;
using ecs::shape2d_component;
using ecs::path_component;
using ecs::polyline_component;
using ecs::rectangle_component;
using ecs::circle_component;
using ecs::ellipse_component;
using ecs::line_component;
using ecs::triangle_component;
using ecs::polygon_component;
using ecs::arc_component;
using ecs::bezier_curve_component;
using ecs::spline_component;
using ecs::sprite_component;
using ecs::text_2d_component;
using ecs::gradient_component;
using ecs::grid_helper_component;
using ecs::gizmo_component;
using ecs::bounding_box_component;
using ecs::mask_component;
using ecs::rigidbody_component;
using ecs::serial_component;
using ecs::osc_component;
using ecs::audio_source_component;
using ecs::midi_source_component;
using ecs::canvas_effects_component;
using ecs::uv_component;
using ecs::uv_sample_component;
// Music
using ecs::transport_control_component;
using ecs::clock_component;
using ecs::sequencer_component;
using ecs::pattern_component;
using ecs::NoteEventKind;
using ecs::note_event_component;
using ecs::midi_output_component;
using ecs::SwatchColor;
using ecs::SwatchColorType;
using ecs::ColorHarmony;
using ecs::GradientStop;
using ecs::swatch_library_component;
using ecs::color_gradient_component;

// Generic single-component inspector
template<typename T>
bool inspectComponent(T& comp, const std::string& name = "") {
    std::string label = name.empty() ? typeid(T).name() : name;
    ComponentInspector ci(label);
    inspector::registerProperties(comp, ci);
    return ci.hasProperties() ? ci.draw() : false;
}

template<typename T>
bool inspectComponent(T& comp, entt::registry& registry, entt::entity entity,
                      const std::string& name = "") {
    std::string label = name.empty() ? typeid(T).name() : name;
    ComponentInspector ci(label);
    inspector::registerProperties(comp, ci, registry, entity);
    return ci.hasProperties() ? ci.draw() : false;
}

// Inspect all components of an entity.
// Returns true if any property was changed this frame.
bool inspectEntity(entt::registry& registry, entt::entity entity);

} // namespace inspector
