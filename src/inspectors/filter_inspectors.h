#pragma once

#include "components/filter_components.h"
#include "components/generator_components.h"
#include "components/draw_filter_components.h"
#include "components/eased_pulse_component.h"
#include "components/state_components.h"
#include "components/canvas_effects_component.h"
#include "ComponentInspector.h"

namespace inspector {

void registerProperties(ecs::tint_filter_component& c, ComponentInspector& inspector);
void registerProperties(ecs::invert_filter_component& c, ComponentInspector& inspector);
void registerProperties(ecs::mirror_filter_component& c, ComponentInspector& inspector);
void registerProperties(ecs::color_adjust_component& c, ComponentInspector& inspector);
void registerProperties(ecs::blur_filter_component& c, ComponentInspector& inspector);
void registerProperties(ecs::dither_filter_component& c, ComponentInspector& inspector);
void registerProperties(ecs::rotate_filter_component& c, ComponentInspector& inspector);
void registerProperties(ecs::threshold_filter_component& c, ComponentInspector& inspector);
void registerProperties(ecs::posterize_filter_component& c, ComponentInspector& inspector);
void registerProperties(ecs::noise_filter_component& c, ComponentInspector& inspector);
void registerProperties(ecs::vignette_filter_component& c, ComponentInspector& inspector);
void registerProperties(ecs::chromatic_aberration_component& c, ComponentInspector& inspector);
void registerProperties(ecs::edge_detect_filter_component& c, ComponentInspector& inspector);

void registerProperties(ecs::gradient_generator_component& c, ComponentInspector& inspector);
void registerProperties(ecs::dots_generator_component& c, ComponentInspector& inspector);
void registerProperties(ecs::stripes_generator_component& c, ComponentInspector& inspector);
void registerProperties(ecs::checkerboard_generator_component& c, ComponentInspector& inspector);
void registerProperties(ecs::noise_generator_component& c, ComponentInspector& inspector);

void registerProperties(ecs::pixelate_filter_component& c, ComponentInspector& inspector);
void registerProperties(ecs::mesh_filter_component& c, ComponentInspector& inspector);
void registerProperties(ecs::rings_filter_component& c, ComponentInspector& inspector);
void registerProperties(ecs::noise_displacement_component& c, ComponentInspector& inspector);
void registerProperties(ecs::line_scan_filter_component& c, ComponentInspector& inspector);
void registerProperties(ecs::ascii_filter_component& c, ComponentInspector& inspector);

void registerProperties(ecs::eased_pulse_component& c, ComponentInspector& inspector);
void registerProperties(ecs::state_preset_component& c, ComponentInspector& inspector);
void registerProperties(ecs::state_library_component& c, ComponentInspector& inspector);
void registerProperties(ecs::state_timeline_component& c, ComponentInspector& inspector);
void registerProperties(ecs::state_morph_component& c, ComponentInspector& inspector);

void registerProperties(ecs::canvas_effects_component& c, ComponentInspector& inspector);

} // namespace inspector
