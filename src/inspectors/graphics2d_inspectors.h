#pragma once

#include <ofxEnTTKit_all.h>
#include "imgui.h"
#include "ComponentInspector.h"

namespace inspector {

void registerProperties(ecs::shape2d_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::path_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::polyline_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::rectangle_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::circle_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::ellipse_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::line_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::triangle_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::polygon_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::arc_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::bezier_curve_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::spline_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::sprite_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::text_2d_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::grid_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::progress_bar_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::arrow_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::star_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::regular_polygon_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::ring_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::cross_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::heart_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::vesica_piscis_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::flower_of_life_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::metatrons_cube_component& comp, ComponentInspector& inspector);
void registerProperties(ecs::soft_mask_component& comp, ComponentInspector& inspector);

} // namespace inspector
