#pragma once

#include <ofxEnTTKit.h>
#include <entt/entity/fwd.hpp>
#include "imgui.h"
#include "ComponentInspector.h"
#include <sstream>
#include <iomanip>

namespace inspector {

void registerProperties(ecs::uv_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::uv_component& comp, ComponentInspector& inspector,
                        entt::registry& registry, entt::entity entity);

void registerProperties(ecs::uv_sample_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::uv_sample_component& comp, ComponentInspector& inspector,
                        entt::registry& registry, entt::entity entity);

} // namespace inspector
