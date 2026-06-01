#pragma once

#include "ecs/swatch_components.h"
#include "ComponentInspector.h"

namespace inspector {

void registerProperties(ecs::swatch_library_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::color_gradient_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::swatch_palette_ref_component& comp, ComponentInspector& inspector);

} // namespace inspector
