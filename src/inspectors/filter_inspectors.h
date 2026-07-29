#pragma once

#include "components/eased_pulse_component.h"
#include "components/state_components.h"
#include "ComponentInspector.h"

namespace inspector {

void registerProperties(ecs::eased_pulse_component& c, ComponentInspector& inspector);
void registerProperties(ecs::state_preset_component& c, ComponentInspector& inspector);
void registerProperties(ecs::state_library_component& c, ComponentInspector& inspector);
void registerProperties(ecs::state_timeline_component& c, ComponentInspector& inspector);
void registerProperties(ecs::state_morph_component& c, ComponentInspector& inspector);

} // namespace inspector
