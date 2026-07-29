#include "filter_inspectors.h"
#include "imgui.h"

namespace inspector {

static const char* s_easingTypeNames[] = {
    "Linear",
    "Ease In Quad", "Ease Out Quad", "Ease In/Out Quad",
    "Ease In Cubic", "Ease Out Cubic", "Ease In/Out Cubic",
    "Ease In Quart", "Ease Out Quart", "Ease In/Out Quart",
    "Ease In Quint", "Ease Out Quint", "Ease In/Out Quint",
    "Ease In Sine", "Ease Out Sine", "Ease In/Out Sine",
    "Ease In Expo", "Ease Out Expo", "Ease In/Out Expo",
    "Ease In Circ", "Ease Out Circ", "Ease In/Out Circ",
    "Ease In Elastic", "Ease Out Elastic", "Ease In/Out Elastic",
    "Ease In Back", "Ease Out Back", "Ease In/Out Back",
    "Ease In Bounce", "Ease Out Bounce", "Ease In/Out Bounce"
};

void registerProperties(ecs::eased_pulse_component& c, ComponentInspector& inspector) {
    inspector.addProperty("Playing", &c.playing);
    inspector.addProperty("Frequency", &c.frequency, 0.01f, 10.0f);
    inspector.addProperty("Duty Cycle", &c.dutyCycle, 0.01f, 0.99f);
    inspector.addProperty("Rise Time", &c.riseTime, 0.01f, 2.0f);
    inspector.addProperty("Fall Time", &c.fallTime, 0.01f, 2.0f);
    inspector.addCustomProperty("Rise Easing", [&c]() {
        int type = (int)c.riseEasing;
        if (ImGui::Combo("##riseEase", &type, s_easingTypeNames, IM_ARRAYSIZE(s_easingTypeNames))) {
            c.riseEasing = (ecs::EasingType)type;
        }
    });
    inspector.addCustomProperty("Fall Easing", [&c]() {
        int type = (int)c.fallEasing;
        if (ImGui::Combo("##fallEase", &type, s_easingTypeNames, IM_ARRAYSIZE(s_easingTypeNames))) {
            c.fallEasing = (ecs::EasingType)type;
        }
    });
    inspector.addProperty("Min Value", &c.minValue);
    inspector.addProperty("Max Value", &c.maxValue);
    inspector.addCustomProperty("Value", [&c]() {
        ImGui::ProgressBar(c.getNormalizedValue(), ImVec2(-1, 0), "");
        ImGui::SameLine();
        ImGui::Text("%.2f", c.value);
    });
    inspector.addCustomProperty("Controls", [&c]() {
        if (ImGui::Button("Play")) c.play();
        ImGui::SameLine();
        if (ImGui::Button("Pause")) c.pause();
        ImGui::SameLine();
        if (ImGui::Button("Reset")) c.reset();
    });
    inspector.addCustomProperty("Bindings", [&c]() {
        ImGui::Text("%d bindings", (int)c.bindings.size());
    });
}

void registerProperties(ecs::state_preset_component& c, ComponentInspector& inspector) {
    inspector.addProperty("Name", &c.name);
    inspector.addProperty("Preview Color", &c.previewColor);
    inspector.addCustomProperty("Info", [&c]() {
        ImGui::Text("Entities: %d", c.getEntityCount());
        ImGui::Text("Properties: %d", c.getPropertyCount());
    });
    inspector.addCustomProperty("Actions", [&c]() {
        if (ImGui::Button("Clear")) {
            c.clear();
        }
    });
}

void registerProperties(ecs::state_library_component& c, ComponentInspector& inspector) {
    inspector.addProperty("Name", &c.name);
    inspector.addCustomProperty("Presets", [&c]() {
        ImGui::Text("%d presets", c.getPresetCount());
        for (int i = 0; i < c.getPresetCount(); i++) {
            ImGui::PushID(i);
            auto* preset = c.getPreset(i);
            bool selected = (c.currentPresetIndex == i);
            ImVec4 color(preset->previewColor.r / 255.0f, preset->previewColor.g / 255.0f,
                        preset->previewColor.b / 255.0f, preset->previewColor.a / 255.0f);
            ImGui::ColorButton("##col", color, ImGuiColorEditFlags_NoTooltip, ImVec2(16, 16));
            ImGui::SameLine();
            if (ImGui::Selectable(preset->name.c_str(), selected)) {
                c.currentPresetIndex = i;
            }
            ImGui::PopID();
        }
        if (ImGui::Button("+ Add Preset")) {
            c.addPreset("State " + std::to_string(c.getPresetCount() + 1));
        }
    });
}

void registerProperties(ecs::state_timeline_component& c, ComponentInspector& inspector) {
    inspector.addProperty("Duration", &c.duration, 1.0f, 120.0f);
    inspector.addProperty("Loop", &c.loop);
    inspector.addProperty("Playback Speed", &c.playbackSpeed, 0.1f, 4.0f);
    inspector.addCustomProperty("Playhead", [&c]() {
        float playhead = c.playhead;
        if (ImGui::SliderFloat("##playhead", &playhead, 0.0f, c.getEffectiveDuration())) {
            c.setPlayhead(playhead);
        }
    });
    inspector.addProperty("Sync to BPM", &c.syncToBPM);
    if (c.syncToBPM) {
        inspector.addProperty("BPM", &c.bpm, 20.0f, 300.0f);
        inspector.addProperty("Beats per Loop", &c.beatsPerLoop, 1, 32);
    }
    inspector.addCustomProperty("Controls", [&c]() {
        if (ImGui::Button(c.playing ? "Pause" : "Play")) {
            if (c.playing) c.pause();
            else c.play();
        }
        ImGui::SameLine();
        if (ImGui::Button("Stop")) {
            c.stop();
        }
    });
    inspector.addCustomProperty("Keyframes", [&c]() {
        ImGui::Text("%d keyframes", c.getKeyframeCount());
    });
}

void registerProperties(ecs::state_morph_component& c, ComponentInspector& inspector) {
    inspector.addProperty("Active", &c.active);
    inspector.addProperty("Duration", &c.duration, 0.1f, 10.0f);
    inspector.addCustomProperty("Easing", [&c]() {
        int type = (int)c.easing;
        if (ImGui::Combo("##morphEase", &type, s_easingTypeNames, IM_ARRAYSIZE(s_easingTypeNames))) {
            c.easing = (ecs::EasingType)type;
        }
    });
    inspector.addCustomProperty("Progress", [&c]() {
        ImGui::ProgressBar(c.progress, ImVec2(-1, 0), "");
        ImGui::SameLine();
        ImGui::Text("%.1f%%", c.progress * 100.0f);
    });
    inspector.addCustomProperty("Controls", [&c]() {
        if (ImGui::Button("Cancel")) {
            c.cancel();
        }
        ImGui::SameLine();
        if (ImGui::Button("Reset")) {
            c.reset();
        }
    });
}

} // namespace inspector
