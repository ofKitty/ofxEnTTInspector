#pragma once

#include <ofxEnTTKit_all.h>
#include "ComponentInspector.h"

namespace inspector {

void registerProperties(ecs::clickable_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::click_bind_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::flag_component& comp, ComponentInspector& inspector);

/// Install ClickableSystem bind hook that writes via PropertyReflector / PropertyValue.
void installClickBindReflectorHook();

} // namespace inspector
