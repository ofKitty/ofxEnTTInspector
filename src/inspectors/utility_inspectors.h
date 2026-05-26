#pragma once

#include <ofxEnTTKit_all.h>
#include "imgui.h"
#include "ComponentInspector.h"

namespace inspector {

void registerProperties(ecs::grid_helper_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::gizmo_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::bounding_box_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::mask_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::rigidbody_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::layer_component& comp, ComponentInspector& inspector);

} // namespace inspector
