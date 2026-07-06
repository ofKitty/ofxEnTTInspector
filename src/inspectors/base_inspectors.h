#pragma once

#include <ofxEnTTKit_all.h>
#include "imgui.h"
#include "ComponentInspector.h"
#include "TransformOriginComponent.h"
#include <map>
#include <fstream>
#include <cstring>

namespace inspector {

void registerProperties(ecs::LocalTransform& comp, ComponentInspector& inspector);
void registerProperties(ecs::skew_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::Relationship& comp, ComponentInspector& inspector,
                        entt::registry& registry, entt::entity entity);

void registerProperties(ecs::named_entity& comp, ComponentInspector& inspector);
void registerProperties(ecs::named_entity& comp, ComponentInspector& inspector, entt::registry& registry, entt::entity entity);

void registerProperties(ecs::fbo_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::fbo_component& comp, ComponentInspector& inspector, entt::registry& registry, entt::entity entity);

void registerProperties(ecs::fbo_reference_component& comp, ComponentInspector& inspector, entt::registry& registry, entt::entity entity);

void registerProperties(ecs::camera_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::render_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::selectable_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::tag_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::mesh_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::image_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::image_component& comp, ComponentInspector& inspector, entt::registry& registry, entt::entity entity);
void registerProperties(ecs::video_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::video_component& comp, ComponentInspector& inspector, entt::registry& registry, entt::entity entity);
void registerProperties(ecs::model_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::model_component& comp, ComponentInspector& inspector, entt::registry& registry, entt::entity entity);
void registerProperties(ecs::resource_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::resource_component& comp, ComponentInspector& inspector, entt::registry& registry, entt::entity entity);

void registerProperties(ecs::canvas_effects_component& comp, ComponentInspector& inspector, entt::registry& registry, entt::entity entity);

} // namespace inspector
