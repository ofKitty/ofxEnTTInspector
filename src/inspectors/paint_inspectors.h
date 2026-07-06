#pragma once

// ============================================================================
// Paint inspectors (ofxEnTTInspector)
// ============================================================================
// ImGui editing for the paint components defined in ofxEnTTKit
// (ecs::solid_color_component / gradient_component / fill_component /
// stroke_component). Gradient editing uses this addon's ImGradientHDR widget.
//
// registerPaintInspectors() wires:
//   * entity-inspector rows (via inspector::addExtraEntityInspector),
//   * "Add Component" picker rows (ecs::registerComponent).
// It runs automatically at static-init time and is idempotent, so explicit
// calls are also safe. The gradient *generator* (full-area rasterization) is a
// render concern and lives in ofxEnTTKit (finalizeGenerators), keeping the
// paint data layer free of any UI dependency.
// ============================================================================

namespace inspector {

void registerPaintInspectors();

} // namespace inspector
