#pragma once

#include <ofxEnTTKit.h>
#include "imgui.h"
#include "ComponentInspector.h"

namespace inspector {

void registerProperties(ecs::tween_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::particle_emitter_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::postfx_component& comp, ComponentInspector& inspector);

} // namespace inspector
