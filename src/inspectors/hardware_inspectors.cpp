#include "hardware_inspectors.h"
#include "VisitFieldsInspector.h"

namespace inspector {

void registerProperties(ecs::serial_component& comp, ComponentInspector& inspector) {
	registerVisitFields(comp, inspector);
}

void registerProperties(ecs::osc_component& comp, ComponentInspector& inspector) {
	registerVisitFields(comp, inspector);
}

void registerProperties(ecs::gpio_component& comp, ComponentInspector& inspector) {
	registerVisitFields(comp, inspector);
	inspector.addCustomProperty("State", [&comp]() {
		ImGui::Text("%s", comp.state ? "HIGH" : "LOW");
		if (comp.justPressed) ImGui::TextColored(ImVec4(0.2f, 1.f, 0.4f, 1.f), "Just pressed");
		if (comp.justReleased) ImGui::TextColored(ImVec4(1.f, 0.6f, 0.2f, 1.f), "Just released");
	});
}

void registerProperties(ecs::network_device_component& comp, ComponentInspector& inspector) {
	registerVisitFields(comp, inspector);
	inspector.addCustomProperty("Status", [&comp]() {
		if (comp.online)
			ImGui::TextColored(ImVec4(0.2f, 1.f, 0.4f, 1.f), "Online  RSSI %d  RTT %.1f ms",
			                   (int)comp.rssi, comp.rttMs);
		else
			ImGui::TextDisabled("Offline");
		if (!comp.firmwareVersion.empty())
			ImGui::Text("Firmware: %s", comp.firmwareVersion.c_str());
	});
}

void registerProperties(ecs::sacn_output_component& comp, ComponentInspector& inspector) {
	registerVisitFields(comp, inspector);
}

void registerProperties(ecs::audio_source_component& comp, ComponentInspector& inspector) {
    inspector.addProperty("Enabled", &comp.enabled);
    inspector.addProperty("Device Index", &comp.deviceIndex, 0, 16);
    inspector.addProperty("Buffer Size", &comp.bufferSize, 64, 4096);
    inspector.addProperty("Sample Rate", &comp.sampleRate, 8000, 192000);
    inspector.addProperty("Input Gain", &comp.inputGain, 0.0f, 4.0f);
    inspector.addProperty("Smoothing", &comp.smoothing, 0.0f, 0.999f);
    inspector.addProperty("Peak Decay", &comp.peakDecay, 0.9f, 0.9999f);
    inspector.addProperty("FFT Bins", &comp.fftBins, 64, 2048);

    inspector.addCustomProperty("Controls", [&]() {
        if (!comp.streamActive) {
            if (ImGui::Button("Start")) comp.start();
        } else {
            ImGui::TextColored(ImVec4(0, 1, 0, 1), "Active");
            ImGui::SameLine();
            if (ImGui::Button("Stop")) comp.stop();
            ImGui::SameLine();
            if (ImGui::Button("Reset")) comp.reset();
        }
    });

    inspector.addCustomProperty("Volume", [&]() {
        ImGui::ProgressBar(comp.smoothedVolume, ImVec2(-1, 0), "");
        ImGui::SameLine(0, 4);
        ImGui::Text("Peak: %.2f", comp.peakVolume);
    });

    static const char* bandNames[] = { "Sub", "Bass", "LoMid", "Mid", "HiMid", "Pres", "Brill" };
    inspector.addCustomProperty("FFT Bands", [&]() {
        for (int i = 0; i < (int)ecs::FFTBand::COUNT; i++) {
            float v = comp.smoothedBandValues[i];
            ImGui::ProgressBar(v, ImVec2(-1, 10));
            ImGui::SameLine(0, 4);
            ImGui::TextUnformatted(bandNames[i]);
        }
    });

    inspector.addCustomProperty("Beat Detectors", [&]() {
        for (int i = 0; i < (int)ecs::FFTBand::COUNT; i++) {
            auto& bd = comp.beatDetectors[i];
            ImGui::PushID(i);
            ImGui::DragFloat("Thresh", &bd.threshold, 0.01f, 0.1f, 5.0f);
            ImGui::SameLine();
            ImGui::DragFloat("Decay", &bd.decay, 0.001f, 0.5f, 0.999f);
            ImGui::SameLine();
            if (bd.triggered) ImGui::TextColored(ImVec4(1,0.6f,0,1), "* %s", bandNames[i]);
            else             ImGui::TextDisabled("  %s", bandNames[i]);
            ImGui::PopID();
        }
    });
}

void registerProperties(ecs::midi_source_component& comp, ComponentInspector& inspector) {
    inspector.addProperty("Enabled", &comp.enabled);
    inspector.addProperty("Device Index", &comp.deviceIndex, 0, 16);
    inspector.addProperty("Listen Channel", &comp.listenChannel, -1, 16);
    inspector.addProperty("CC Smoothing", &comp.ccSmoothing, 0.0f, 0.999f);
    inspector.addProperty("Learn Mode", &comp.learnMode);

    inspector.addCustomProperty("Controls", [&]() {
        if (!comp.streamActive) {
            if (ImGui::Button("Start")) comp.start();
        } else {
            ImGui::TextColored(ImVec4(0, 1, 0, 1), "Active");
            ImGui::SameLine();
            if (ImGui::Button("Stop")) comp.stop();
            ImGui::SameLine();
            if (ImGui::Button("Reset")) comp.reset();
        }
    });

    inspector.addCustomProperty("Status", [&]() {
        ImGui::Text("Notes active: %d  Last note: %d  Velocity: %d",
            comp.activeNoteCount, comp.lastNoteOn, comp.lastVelocity);
        ImGui::ProgressBar((comp.pitchBend + 1.0f) * 0.5f, ImVec2(-1, 6), "");
        ImGui::SameLine(0, 4); ImGui::TextDisabled("PB");
        ImGui::ProgressBar(comp.smoothedModWheel, ImVec2(-1, 6), "");
        ImGui::SameLine(0, 4); ImGui::TextDisabled("MW");
        ImGui::ProgressBar(comp.smoothedExpression, ImVec2(-1, 6), "");
        ImGui::SameLine(0, 4); ImGui::TextDisabled("Expr");
    });

    inspector.addCustomProperty("CC Monitor", [&]() {
        ImGui::BeginChild("##ccmon", ImVec2(0, 120), false, ImGuiWindowFlags_HorizontalScrollbar);
        for (int i = 0; i < 128; i++) {
            if (comp.smoothedCCValues[i] > 0.001f) {
                ImGui::Text("CC%3d: %.2f", i, comp.smoothedCCValues[i]);
                ImGui::SameLine();
                ImGui::ProgressBar(comp.smoothedCCValues[i], ImVec2(80, 10), "");
            }
        }
        ImGui::EndChild();
    });

    inspector.addCustomProperty("Learn", [&]() {
        static char learnName[64] = "";
        ImGui::InputText("Target Name", learnName, 64);
        ImGui::SameLine();
        if (ImGui::Button("Start Learn")) comp.startLearn(learnName);
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) comp.cancelLearn();
        if (!comp.learnTargetName.empty())
            ImGui::TextColored(ImVec4(1, 0.8f, 0, 1), "Learning: %s", comp.learnTargetName.c_str());
    });
}

} // namespace inspector
