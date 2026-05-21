#include "entity_inspector.h"

namespace inspector {

bool inspectEntity(entt::registry& registry, entt::entity entity) {
    if (!registry.valid(entity)) { ImGui::Text("Invalid entity"); return false; }

    bool changed = false;
    ImGui::PushID((int)entity);

    // ── Base ─────────────────────────────────────────────────────────────────
    if (registry.any_of<node_component>(entity))
        changed |= inspectComponent(registry.get<node_component>(entity), registry, entity, "Node");

    if (registry.any_of<render_component>(entity))
        changed |= inspectComponent(registry.get<render_component>(entity), "Render");

    if (registry.any_of<camera_component>(entity))
        changed |= inspectComponent(registry.get<camera_component>(entity), "Camera");

    if (registry.any_of<fbo_component>(entity)) {
        auto& comp = registry.get<fbo_component>(entity);
        ComponentInspector ci("FBO");
        inspector::registerProperties(comp, ci, registry, entity);
        if (ci.hasProperties()) changed |= ci.draw();
    }

    if (registry.any_of<fbo_reference_component>(entity)) {
        auto& comp = registry.get<fbo_reference_component>(entity);
        ComponentInspector ci("Canvas Instance");
        inspector::registerProperties(comp, ci, registry, entity);
        if (ci.hasProperties()) changed |= ci.draw();
    }

    if (registry.any_of<canvas_effects_component>(entity)) {
        auto& comp = registry.get<canvas_effects_component>(entity);
        ComponentInspector ci("Canvas Effects");
        inspector::registerProperties(comp, ci, registry, entity);
        if (ci.hasProperties()) changed |= ci.draw();
    }

    if (registry.any_of<tag_component>(entity))
        changed |= inspectComponent(registry.get<tag_component>(entity), "Tag");

    if (registry.any_of<selectable_component>(entity))
        changed |= inspectComponent(registry.get<selectable_component>(entity), "Selectable");

    if (registry.any_of<mesh_component>(entity))
        changed |= inspectComponent(registry.get<mesh_component>(entity), "Mesh");

    if (registry.any_of<model_component>(entity))
        changed |= inspectComponent(registry.get<model_component>(entity), registry, entity, "Model");

    if (registry.any_of<image_component>(entity))
        changed |= inspectComponent(registry.get<image_component>(entity), registry, entity, "Image");

    if (registry.any_of<video_component>(entity))
        changed |= inspectComponent(registry.get<video_component>(entity), registry, entity, "Video");

    if (registry.any_of<resource_component>(entity))
        changed |= inspectComponent(registry.get<resource_component>(entity), registry, entity, "Resource");

    // ── Rendering ────────────────────────────────────────────────────────────
    if (registry.any_of<light_component>(entity))
        changed |= inspectComponent(registry.get<light_component>(entity), "Light");

    if (registry.any_of<material_component>(entity))
        changed |= inspectComponent(registry.get<material_component>(entity), "Material");

    if (registry.any_of<shader_component>(entity))
        changed |= inspectComponent(registry.get<shader_component>(entity), "Shader");

    if (registry.any_of<primitive_component>(entity))
        changed |= inspectComponent(registry.get<primitive_component>(entity), "Primitive");

    if (registry.any_of<texture_component>(entity))
        changed |= inspectComponent(registry.get<texture_component>(entity), "Texture");

    if (registry.any_of<cubemap_component>(entity))
        changed |= inspectComponent(registry.get<cubemap_component>(entity), "Cubemap");

    if (registry.any_of<billboard_component>(entity))
        changed |= inspectComponent(registry.get<billboard_component>(entity), "Billboard");

    if (registry.any_of<trail_component>(entity))
        changed |= inspectComponent(registry.get<trail_component>(entity), "Trail");

    if (registry.any_of<tube_component>(entity))
        changed |= inspectComponent(registry.get<tube_component>(entity), "Tube");

    if (registry.any_of<surface_component>(entity))
        changed |= inspectComponent(registry.get<surface_component>(entity), "Surface");

    if (registry.any_of<instanced_mesh_component>(entity))
        changed |= inspectComponent(registry.get<instanced_mesh_component>(entity), "Instanced Mesh");

    if (registry.any_of<outline_component>(entity))
        changed |= inspectComponent(registry.get<outline_component>(entity), "Outline");

    if (registry.any_of<shadow_component>(entity))
        changed |= inspectComponent(registry.get<shadow_component>(entity), "Shadow");

    if (registry.any_of<glow_component>(entity))
        changed |= inspectComponent(registry.get<glow_component>(entity), "Glow");

    // ── Animation ────────────────────────────────────────────────────────────
    if (registry.any_of<tween_component>(entity))
        changed |= inspectComponent(registry.get<tween_component>(entity), "Tween");

    if (registry.any_of<particle_emitter_component>(entity))
        changed |= inspectComponent(registry.get<particle_emitter_component>(entity), "Particle Emitter");

    if (registry.any_of<postfx_component>(entity))
        changed |= inspectComponent(registry.get<postfx_component>(entity), "PostFX");

    // ── 2D Graphics ──────────────────────────────────────────────────────────
    if (registry.any_of<shape2d_component>(entity)) {
        auto& comp = registry.get<shape2d_component>(entity);
        changed |= inspectComponent(comp, ecs::getShapeTypeName(comp.type));
    }

    // ── Utility ──────────────────────────────────────────────────────────────
    if (registry.any_of<grid_helper_component>(entity))
        changed |= inspectComponent(registry.get<grid_helper_component>(entity), "Grid Helper");

    if (registry.any_of<gizmo_component>(entity))
        changed |= inspectComponent(registry.get<gizmo_component>(entity), "Gizmo");

    if (registry.any_of<bounding_box_component>(entity))
        changed |= inspectComponent(registry.get<bounding_box_component>(entity), "Bounding Box");

    if (registry.any_of<mask_component>(entity))
        changed |= inspectComponent(registry.get<mask_component>(entity), "Mask");

    if (registry.any_of<rigidbody_component>(entity))
        changed |= inspectComponent(registry.get<rigidbody_component>(entity), "Rigidbody");

    // ── Hardware ─────────────────────────────────────────────────────────────
    if (registry.any_of<serial_component>(entity))
        changed |= inspectComponent(registry.get<serial_component>(entity), "Serial");

    if (registry.any_of<osc_component>(entity))
        changed |= inspectComponent(registry.get<osc_component>(entity), "OSC");

    if (registry.any_of<audio_source_component>(entity))
        changed |= inspectComponent(registry.get<audio_source_component>(entity), "Audio Source");

    if (registry.any_of<midi_source_component>(entity))
        changed |= inspectComponent(registry.get<midi_source_component>(entity), "MIDI Source");

    // ── LED / UV ─────────────────────────────────────────────────────────────
    if (registry.any_of<uv_component>(entity)) {
        auto& comp = registry.get<uv_component>(entity);
        ComponentInspector ci("UV Mapping");
        inspector::registerProperties(comp, ci, registry, entity);
        if (ci.hasProperties()) changed |= ci.draw();
    }

    if (registry.any_of<uv_sample_component>(entity)) {
        auto& comp = registry.get<uv_sample_component>(entity);
        ComponentInspector ci("UV Sampler");
        inspector::registerProperties(comp, ci, registry, entity);
        if (ci.hasProperties()) changed |= ci.draw();
    }

    // ── Swatches ─────────────────────────────────────────────────────────────
    if (registry.any_of<swatch_library_component>(entity))
        changed |= inspectComponent(registry.get<swatch_library_component>(entity), "Swatch Library");

    if (registry.any_of<color_gradient_component>(entity))
        changed |= inspectComponent(registry.get<color_gradient_component>(entity), "Color Gradient");

    // ── Filters ──────────────────────────────────────────────────────────────
    #define INSPECT_FILTER(T, label) \
        if (registry.any_of<ecs::T>(entity)) { \
            ComponentInspector ci(label); \
            inspector::registerProperties(registry.get<ecs::T>(entity), ci); \
            changed |= ci.draw(); \
        }

    INSPECT_FILTER(tint_filter_component,            "Tint Filter")
    INSPECT_FILTER(invert_filter_component,          "Invert Filter")
    INSPECT_FILTER(mirror_filter_component,          "Mirror Filter")
    INSPECT_FILTER(color_adjust_component,           "Color Adjust")
    INSPECT_FILTER(blur_filter_component,            "Blur Filter")
    INSPECT_FILTER(dither_filter_component,          "Dither Filter")
    INSPECT_FILTER(rotate_filter_component,          "Rotate Filter")
    INSPECT_FILTER(threshold_filter_component,       "Threshold Filter")
    INSPECT_FILTER(posterize_filter_component,       "Posterize Filter")
    INSPECT_FILTER(noise_filter_component,           "Noise Filter")
    INSPECT_FILTER(vignette_filter_component,        "Vignette Filter")
    INSPECT_FILTER(chromatic_aberration_component,   "Chromatic Aberration")
    INSPECT_FILTER(pixelate_filter_component,        "Pixelate Filter")
    INSPECT_FILTER(mesh_filter_component,            "Mesh Filter")
    INSPECT_FILTER(rings_filter_component,           "Rings Filter")
    INSPECT_FILTER(line_scan_filter_component,       "Line Scan Filter")
    INSPECT_FILTER(ascii_filter_component,           "ASCII Filter")

    // ── Generators ───────────────────────────────────────────────────────────
    INSPECT_FILTER(gradient_generator_component,     "Gradient Generator")
    INSPECT_FILTER(dots_generator_component,         "Dots Generator")
    INSPECT_FILTER(stripes_generator_component,      "Stripes Generator")
    INSPECT_FILTER(checkerboard_generator_component, "Checkerboard Generator")
    INSPECT_FILTER(noise_generator_component,        "Noise Generator")

    // ── Modulators ───────────────────────────────────────────────────────────
    INSPECT_FILTER(eased_pulse_component,            "Eased Pulse")
    INSPECT_FILTER(state_preset_component,           "State Preset")
    INSPECT_FILTER(state_library_component,          "State Library")
    INSPECT_FILTER(state_timeline_component,         "State Timeline")
    INSPECT_FILTER(state_morph_component,            "State Morph")

    #undef INSPECT_FILTER

    // ── Music ────────────────────────────────────────────────────────────────
    if (registry.any_of<transport_control_component>(entity))
        changed |= inspectComponent(registry.get<transport_control_component>(entity), "Transport");

    if (registry.any_of<clock_component>(entity))
        changed |= inspectComponent(registry.get<clock_component>(entity), "Clock");

    if (registry.any_of<sequencer_component>(entity))
        changed |= inspectComponent(registry.get<sequencer_component>(entity), "Sequencer");

    if (registry.any_of<pattern_component>(entity))
        changed |= inspectComponent(registry.get<pattern_component>(entity), "Pattern");

    if (registry.any_of<midi_output_component>(entity))
        changed |= inspectComponent(registry.get<midi_output_component>(entity), "MIDI Output");

    if (registry.any_of<trigger_lane_component>(entity))
        changed |= inspectComponent(registry.get<trigger_lane_component>(entity), "Trigger Lane");

    if (registry.any_of<trigger_pattern_component>(entity))
        changed |= inspectComponent(registry.get<trigger_pattern_component>(entity), "Trigger Pattern");

    if (registry.any_of<trigger_pattern_data_component>(entity))
        changed |= inspectComponent(registry.get<trigger_pattern_data_component>(entity), "Trigger Pattern Grid");

    if (registry.any_of<trigger_sequencer_component>(entity))
        changed |= inspectComponent(registry.get<trigger_sequencer_component>(entity), "Trigger Sequencer");

    ImGui::PopID();
    return changed;
}

} // namespace inspector
