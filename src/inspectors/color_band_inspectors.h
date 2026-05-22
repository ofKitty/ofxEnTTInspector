#pragma once

#include "ComponentInspector.h"
#include "components/color_band_component.h"

namespace inspector {

void registerProperties(ecs::color_band_component& band, ComponentInspector& inspector);

} // namespace inspector
