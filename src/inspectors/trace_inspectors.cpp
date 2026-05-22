#include "trace_inspectors.h"

namespace inspector {

void registerProperties(ecs::greyscale_threshold_settings& s, ComponentInspector& inspector) {
	inspector.addProperty("Value Min", &s.valueMin, 0, 255);
	inspector.addProperty("Value Max", &s.valueMax, 0, 255);
	inspector.addProperty("Invert Mask", &s.invert);
}

void registerProperties(ecs::curve_trace_settings& s, ComponentInspector& inspector) {
	inspector.addProperty("Turd Size", &s.turdsize, 0, 100);
	inspector.addProperty("Alphamax", &s.alphamax, 0.0f, 1.34f, 0.05f);
	inspector.addProperty("Curve Optimize", &s.opticurve);
	inspector.addProperty("Opt Tolerance", &s.opttolerance, 0.0f, 2.0f, 0.01f);
	inspector.addProperty("Curve Resolution", &s.curveResolution, 2, 128);
	inspector.addProperty("Trace Scale", &s.traceScale, 1.0f, 8.0f, 0.25f);
	inspector.addProperty("Trace Holes", &s.traceHoles);
}

} // namespace inspector
