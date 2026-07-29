#include "entity_inspector.h"

namespace inspector {

bool inspectEntity(entt::registry& registry, entt::entity entity) {
    if (!registry.valid(entity)) { ImGui::Text("Invalid entity"); return false; }

    bool changed = false;
    ImGui::PushID((int)entity);

    // ── Hierarchy (ofxNode / layers) ─────────────────────────────────────────
    if (registry.any_of<ecs::LocalTransform>(entity))
        changed |= inspectComponent(registry.get<ecs::LocalTransform>(entity), "Local Transform");

    if (registry.any_of<ecs::skew_component>(entity))
        changed |= inspectComponent(registry.get<ecs::skew_component>(entity), "Skew");

    if (registry.any_of<ecs::Relationship>(entity)) {
        auto& comp = registry.get<ecs::Relationship>(entity);
        ComponentInspector ci("Relationship");
        inspector::registerProperties(comp, ci, registry, entity);
        if (ci.hasProperties()) changed |= ci.draw();
    }

    // ── Base ─────────────────────────────────────────────────────────────────
    if (registry.any_of<named_entity>(entity))
        changed |= inspectComponent(registry.get<named_entity>(entity), registry, entity, "Name");

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

    if (registry.any_of<tag_component>(entity))
        changed |= inspectComponent(registry.get<tag_component>(entity), "Tag");

    if (registry.any_of<code_snippet_component>(entity))
        changed |= inspectComponent(registry.get<code_snippet_component>(entity), "Code");

    if (registry.any_of<selectable_component>(entity)) {
        // Internal sync flag — not shown in Properties.
    }

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

    if (registry.any_of<projector_component>(entity))
        changed |= inspectComponent(registry.get<projector_component>(entity), "Projector");

    if (registry.any_of<material_component>(entity))
        changed |= inspectComponent(registry.get<material_component>(entity), "Material");

    if (registry.any_of<shader_component>(entity))
        changed |= inspectComponent(registry.get<shader_component>(entity), "Shader");

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
    if (registry.any_of<path_component>(entity))
        changed |= inspectComponent(registry.get<path_component>(entity), "Path");

    if (registry.any_of<corner_radius_component>(entity)) {
        const bool crChanged = inspectComponent(
            registry.get<corner_radius_component>(entity), "Corner Radius");
        if (crChanged)
            ecs::invalidateRoundedPathCache(registry, entity);
        changed |= crChanged;
    }

    if (registry.any_of<curve_resolution_component>(entity))
        changed |= inspectComponent(registry.get<curve_resolution_component>(entity), "Curve Resolution");

    if (registry.any_of<polyline_component>(entity))
        changed |= inspectComponent(registry.get<polyline_component>(entity), "Polyline");

    if (registry.any_of<rectangle_component>(entity))
        changed |= inspectComponent(registry.get<rectangle_component>(entity), "Rectangle");

    if (registry.any_of<circle_component>(entity))
        changed |= inspectComponent(registry.get<circle_component>(entity), "Circle");

    if (registry.any_of<ellipse_component>(entity))
        changed |= inspectComponent(registry.get<ellipse_component>(entity), "Ellipse");

    if (registry.any_of<line_component>(entity))
        changed |= inspectComponent(registry.get<line_component>(entity), "Line");

    if (registry.any_of<triangle_component>(entity))
        changed |= inspectComponent(registry.get<triangle_component>(entity), "Triangle");

    if (registry.any_of<polygon_component>(entity))
        changed |= inspectComponent(registry.get<polygon_component>(entity), "Polygon");

    if (registry.any_of<arc_component>(entity))
        changed |= inspectComponent(registry.get<arc_component>(entity), "Arc");

    if (registry.any_of<bezier_curve_component>(entity))
        changed |= inspectComponent(registry.get<bezier_curve_component>(entity), "Bezier Curve");

    if (registry.any_of<spline_component>(entity))
        changed |= inspectComponent(registry.get<spline_component>(entity), "Spline");

    if (registry.any_of<sprite_component>(entity))
        changed |= inspectComponent(registry.get<sprite_component>(entity), "Sprite");

    if (registry.any_of<text_2d_component>(entity))
        changed |= inspectComponent(registry.get<text_2d_component>(entity), "Text 2D");

    if (registry.any_of<star_component>(entity))
        changed |= inspectComponent(registry.get<star_component>(entity), "Star");

    if (registry.any_of<regular_polygon_component>(entity))
        changed |= inspectComponent(registry.get<regular_polygon_component>(entity), "Regular Polygon");

    if (registry.any_of<ring_component>(entity))
        changed |= inspectComponent(registry.get<ring_component>(entity), "Ring");

    if (registry.any_of<cross_component>(entity))
        changed |= inspectComponent(registry.get<cross_component>(entity), "Cross");

    if (registry.any_of<heart_component>(entity))
        changed |= inspectComponent(registry.get<heart_component>(entity), "Heart");

    if (registry.any_of<vesica_piscis_component>(entity))
        changed |= inspectComponent(registry.get<vesica_piscis_component>(entity), "Vesica Piscis");

    if (registry.any_of<flower_of_life_component>(entity))
        changed |= inspectComponent(registry.get<flower_of_life_component>(entity), "Flower of Life");

    if (registry.any_of<metatrons_cube_component>(entity))
        changed |= inspectComponent(registry.get<metatrons_cube_component>(entity), "Metatron's Cube");

    if (registry.any_of<soft_mask_component>(entity))
        changed |= inspectComponent(registry.get<soft_mask_component>(entity), "Soft Mask");

    if (registry.any_of<grid_component>(entity))
        changed |= inspectComponent(registry.get<grid_component>(entity), "Grid");

    if (registry.any_of<progress_bar_component>(entity))
        changed |= inspectComponent(registry.get<progress_bar_component>(entity), "Progress Bar");

    if (registry.any_of<arrow_component>(entity))
        changed |= inspectComponent(registry.get<arrow_component>(entity), "Arrow");

    // ── Utility ──────────────────────────────────────────────────────────────
    if (registry.any_of<grid_helper_component>(entity))
        changed |= inspectComponent(registry.get<grid_helper_component>(entity), "Grid Helper");

    if (registry.any_of<ecs::layer_component>(entity))
        changed |= inspectComponent(registry.get<ecs::layer_component>(entity), "Layer");

    if (registry.any_of<gizmo_component>(entity))
        changed |= inspectComponent(registry.get<gizmo_component>(entity), "Gizmo");

    if (registry.any_of<bounding_box_component>(entity))
        changed |= inspectComponent(registry.get<bounding_box_component>(entity), "Bounding Box");

    if (registry.any_of<mask_component>(entity))
        changed |= inspectComponent(registry.get<mask_component>(entity), "Mask");

    if (registry.any_of<rigidbody_component>(entity))
        changed |= inspectComponent(registry.get<rigidbody_component>(entity), "Rigidbody");

    // ── Hardware / LED / network (ofxEnTTKit) ────────────────────────────────
    if (registry.any_of<audio_source_component>(entity))
        changed |= inspectComponent(registry.get<audio_source_component>(entity), "Audio Source");

    if (registry.any_of<midi_source_component>(entity))
        changed |= inspectComponent(registry.get<midi_source_component>(entity), "MIDI Source");

    if (registry.any_of<serial_component>(entity))
        changed |= inspectComponent(registry.get<serial_component>(entity), "Serial");

    if (registry.any_of<osc_component>(entity))
        changed |= inspectComponent(registry.get<osc_component>(entity), "OSC");

    if (registry.any_of<gpio_component>(entity))
        changed |= inspectComponent(registry.get<gpio_component>(entity), "GPIO Trigger");

    if (registry.any_of<network_device_component>(entity))
        changed |= inspectComponent(registry.get<network_device_component>(entity), "Network Device");

    if (registry.any_of<sacn_output_component>(entity))
        changed |= inspectComponent(registry.get<sacn_output_component>(entity), "sACN Output");

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

    if (registry.any_of<swatch_palette_ref_component>(entity))
        changed |= inspectComponent(registry.get<swatch_palette_ref_component>(entity), "Swatch Palette Ref");

    // ── Modulators / state ───────────────────────────────────────────────────
    #define INSPECT_FILTER(T, label) \
        if (registry.any_of<ecs::T>(entity)) { \
            ComponentInspector ci(label); \
            inspector::registerProperties(registry.get<ecs::T>(entity), ci); \
            changed |= ci.draw(); \
        }

    INSPECT_FILTER(modulator_component,              "Modulator")
    INSPECT_FILTER(mod_binding_component,            "Mod Binding")
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

    if (extraEntityInspector())
        changed |= extraEntityInspector()(registry, entity);

    for (auto& fn : extraEntityInspectors())
        if (fn) changed |= fn(registry, entity);

    ImGui::PopID();
    return changed;
}

} // namespace inspector
