#pragma once

// ============================================================================
// ImGradientHDR <-> ofJson bridge (ofxEnTTInspector)
// ============================================================================
// Converts between an ImGradientHDR widget state and this JSON gradient schema:
//
//   settings["stops"] = [
//     { "position": 0.0, "color": { "r": 0, "g": 0, "b": 0, "a": 255 } },
//     { "position": 1.0, "color": { "r": 255, "g": 255, "b": 255, "a": 255 } }
//   ]
//
// Colors are 0-255 ints in JSON, 0-1 floats in the widget. HDR intensity is
// folded into the RGB values on write (clamped); the schema has no intensity
// field.
// ============================================================================

#include "ImGradientHDR.h"
#include "ofJson.h"

namespace ofkitty {

/// Populate a widget state from settings["stops"]. Falls back to a
/// black -> white gradient when no valid stops are present.
void loadGradientHDRFromJson(const ofJson& settings, ImGradientHDRState& state);

/// Write the widget state back into settings["stops"] (sorted by position).
void applyGradientHDRToJson(const ImGradientHDRState& state, ofJson& settings);

} // namespace ofkitty
