#pragma once

#include <ofxEnTTKit_all.h>
#include "imgui.h"
#include "ComponentInspector.h"

namespace inspector {

void registerProperties(ecs::mod_binding_component& binding, ComponentInspector& inspector);
void registerProperties(ecs::modulator_component& comp, ComponentInspector& inspector);

} // namespace inspector
