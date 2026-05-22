#include "color_band_inspectors.h"

namespace inspector {

void registerProperties(ecs::color_band_component& band, ComponentInspector& inspector) {
	inspector.addEnumProperty("Model", &band.model, {"HSV", "Lab"});

	inspector.addCustomProperty("Band range", [&band]() {
		if(band.model == ecs::ColorBandModel::Hsv) {
			ImGui::SliderInt("Hue Min", &band.hueMin, 0, 179);
			ImGui::SliderInt("Hue Max", &band.hueMax, 0, 179);
			ImGui::SliderInt("Saturation Min", &band.saturationMin, 0, 255);
			ImGui::SliderInt("Saturation Max", &band.saturationMax, 0, 255);
			ImGui::SliderInt("Value Min", &band.valueMin, 0, 255);
			ImGui::SliderInt("Value Max", &band.valueMax, 0, 255);
			ImGui::Checkbox("Wrap hue", &band.wrapHue);
			if(band.wrapHue) {
				ImGui::TextWrapped("With wrap enabled, Hue Min above Hue Max selects a band around the wheel.");
			}
		} else {
			ImGui::SliderInt("Lightness Min", &band.lightnessMin, 0, 255);
			ImGui::SliderInt("Lightness Max", &band.lightnessMax, 0, 255);
			ImGui::SliderInt("A Min", &band.aMin, 0, 255);
			ImGui::SliderInt("A Max", &band.aMax, 0, 255);
			ImGui::SliderInt("B Min", &band.bMin, 0, 255);
			ImGui::SliderInt("B Max", &band.bMax, 0, 255);
		}
	});
}

} // namespace inspector
