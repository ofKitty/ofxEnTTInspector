#pragma once

#include "ComponentInspector.h"
#include <ofxEnTTKit_all.h>

namespace inspector {

void registerProperties(ecs::code_snippet_component& comp, ComponentInspector& inspector);

} // namespace inspector
