#include "ImGradientHDRJson.h"

#include <algorithm>
#include <vector>

namespace ofkitty {

void loadGradientHDRFromJson(const ofJson& settings, ImGradientHDRState& state)
{
	state = ImGradientHDRState{};

	const ofJson stops = settings.value("stops", ofJson::array());
	if (stops.is_array()) {
		for (const auto& stop : stops) {
			if (!stop.is_object()) continue;
			if (state.ColorCount >= MarkerMax) break;

			const float pos = ofClamp(stop.value("position", 0.f), 0.f, 1.f);
			const ofJson c  = stop.value("color", ofJson::object());
			const std::array<float, 3> rgb = {
				c.value("r", 0) / 255.f,
				c.value("g", 0) / 255.f,
				c.value("b", 0) / 255.f,
			};
			state.AddColorMarker(pos, rgb, 1.f);
			state.AddAlphaMarker(pos, c.value("a", 255) / 255.f);
		}
	}

	if (state.ColorCount == 0) {
		state.AddColorMarker(0.f, { 0.f, 0.f, 0.f }, 1.f);
		state.AddColorMarker(1.f, { 1.f, 1.f, 1.f }, 1.f);
	}
	if (state.AlphaCount == 0) {
		state.AddAlphaMarker(0.f, 1.f);
		state.AddAlphaMarker(1.f, 1.f);
	}
}

void applyGradientHDRToJson(const ImGradientHDRState& state, ofJson& settings)
{
	std::vector<int> order(static_cast<size_t>(std::max(0, state.ColorCount)));
	for (size_t i = 0; i < order.size(); ++i) order[i] = (int)i;
	std::sort(order.begin(), order.end(), [&state](int a, int b) {
		return state.Colors[(size_t)a].Position < state.Colors[(size_t)b].Position;
	});

	ofJson stops = ofJson::array();
	for (int idx : order) {
		const auto& m = state.Colors[(size_t)idx];
		const float alpha = state.GetAlpha(m.Position);
		// Fold HDR intensity into RGB (JSON schema has no intensity field).
		auto channel = [&m](int i) {
			return (int)ofClamp(m.Color[(size_t)i] * m.Intensity * 255.f + 0.5f, 0.f, 255.f);
		};
		stops.push_back({
			{ "position", m.Position },
			{ "color", {
				{ "r", channel(0) },
				{ "g", channel(1) },
				{ "b", channel(2) },
				{ "a", (int)ofClamp(alpha * 255.f + 0.5f, 0.f, 255.f) },
			} },
		});
	}
	settings["stops"] = stops;
}

} // namespace ofkitty
