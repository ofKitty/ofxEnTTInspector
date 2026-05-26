#pragma once

#include <ofxEnTTKit_all.h>
#include "imgui.h"
#include "ComponentInspector.h"

namespace inspector {

void registerProperties(ecs::keyboard_input_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::joystick_input_component& comp, ComponentInspector& inspector);

} // namespace inspector
