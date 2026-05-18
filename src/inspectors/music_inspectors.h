#pragma once

#include <ofxEnTTKit.h>
#include "imgui.h"
#include "../ComponentInspector.h"

namespace inspector {

void registerProperties(ecs::transport_control_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::clock_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::sequencer_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::pattern_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::midi_output_component& comp, ComponentInspector& inspector);

} // namespace inspector
