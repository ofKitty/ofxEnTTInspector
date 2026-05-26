#pragma once

#include <ofxEnTTKit_all.h>
#include "imgui.h"
#include "ComponentInspector.h"

namespace inspector {

void registerProperties(ecs::light_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::material_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::shader_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::primitive_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::texture_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::trail_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::outline_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::shadow_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::glow_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::cubemap_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::billboard_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::tube_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::surface_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::instanced_mesh_component& comp, ComponentInspector& inspector);

} // namespace inspector
