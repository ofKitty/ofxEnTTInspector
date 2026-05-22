#pragma once

#include "ComponentInspector.h"
#include "components/trace_components.h"

namespace inspector {

void registerProperties(ecs::greyscale_threshold_settings& s, ComponentInspector& inspector);
void registerProperties(ecs::curve_trace_settings& s, ComponentInspector& inspector);

} // namespace inspector
