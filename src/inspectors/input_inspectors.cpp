#include "input_inspectors.h"

namespace inspector {

void registerProperties(ecs::keyboard_input_component& comp, ComponentInspector& inspector) {
	inspector.addProperty("Enabled", &comp.enabled);
	inspector.addProperty("Active", &comp.active);

	inspector.addCustomProperty("Arrow Keys", [&]() {
		ImGui::TextDisabled("Up %s  Down %s  Left %s  Right %s",
		                    comp.isDown(OF_KEY_UP) ? "ON" : "off",
		                    comp.isDown(OF_KEY_DOWN) ? "ON" : "off",
		                    comp.isDown(OF_KEY_LEFT) ? "ON" : "off",
		                    comp.isDown(OF_KEY_RIGHT) ? "ON" : "off");
		ImGui::TextDisabled("Space %s  Shift %s",
		                    comp.isDown(' ') ? "ON" : "off",
		                    comp.isDown(OF_KEY_SHIFT) ? "ON" : "off");
	});
}

void registerProperties(ecs::joystick_input_component& comp, ComponentInspector& inspector) {
	inspector.addProperty("Enabled", &comp.enabled);
	inspector.addProperty("Active", &comp.active);
	inspector.addProperty("Joy ID", &comp.joyId, 0, 16);

	inspector.addCustomProperty("Device", [&]() {
		if (comp.connected) {
			ImGui::TextColored(ImVec4(0.2f, 0.9f, 0.4f, 1.f), "%s", comp.name.c_str());
			ImGui::Text("Buttons: %d  Axes: %d", comp.buttonCount, comp.axisCount);
		} else {
			ImGui::TextDisabled("Not connected");
		}
		if (ImGui::Button("Setup Device")) {
			comp.setup(comp.joyId);
		}
	});

	inspector.addCustomProperty("Axes", [&]() {
		for (int i = 0; i < comp.axisCount && i < ecs::joystick_input_component::kMaxAxes; ++i) {
			ImGui::Text("Axis %d", i);
			ImGui::SameLine();
			ImGui::ProgressBar((comp.getAxis(i) + 1.f) * 0.5f, ImVec2(120, 12), "");
		}
	});

	inspector.addCustomProperty("Buttons", [&]() {
		for (int i = 0; i < comp.buttonCount && i < ecs::joystick_input_component::kMaxButtons; ++i) {
			if (comp.isPushing(i)) {
				ImGui::TextColored(ImVec4(0.3f, 0.85f, 1.f, 1.f), "Btn %d pressed", i);
			}
		}
	});
}

} // namespace inspector
