#pragma once

// ============================================================================
// ofxEnTTInspector - Component Inspectors
// ImGui UI layer for editing ofxEnTTKit ECS component data.
//
// Usage:
//   #include "inspectors/inspectors.h"
//   inspector::ComponentInspector ci("MyComp");
//   inspector::registerProperties(myComp, ci);
//   ci.draw();
// ============================================================================

#include "base_inspectors.h"
#include "rendering_inspectors.h"
#include "animation_inspectors.h"
#include "hardware_inspectors.h"
#include "led_inspectors.h"
#include "input_inspectors.h"
#include "interaction_inspectors.h"

#include "utility_inspectors.h"
#include "modulator_inspectors.h"
#include "graphics2d_inspectors.h"
#include "swatch_inspectors.h"
#include "filter_inspectors.h"
#include "music_inspectors.h"
#include "code_snippet_inspectors.h"
#include "paint_inspectors.h"
