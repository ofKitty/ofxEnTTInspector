#pragma once

#include <ofxEnTTKit_all.h>
#include "imgui.h"
#include "ComponentInspector.h"

namespace inspector {

void registerProperties(ecs::serial_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::osc_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::audio_source_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::midi_source_component& comp, ComponentInspector& inspector);

} // namespace inspector
