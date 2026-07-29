#pragma once

#include "components/led_components.h"
#include "ComponentInspector.h"
#include <entt/entity/fwd.hpp>

namespace inspector {

void registerProperties(ecs::uv_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::uv_component& comp, ComponentInspector& inspector,
                        entt::registry& registry, entt::entity entity);

void registerProperties(ecs::uv_sample_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::uv_sample_component& comp, ComponentInspector& inspector,
                        entt::registry& registry, entt::entity entity);

} // namespace inspector
