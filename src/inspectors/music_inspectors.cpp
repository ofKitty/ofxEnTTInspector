#include "music_inspectors.h"

namespace inspector {

// ── Note name helper ─────────────────────────────────────────────────────────
static const char* s_noteNames[] = {
    "C","C#","D","D#","E","F","F#","G","G#","A","A#","B"
};
static const char* midiNoteName(int note)
{
    if (note < 0 || note > 127) return "?";
    return s_noteNames[note % 12];
}
static int midiNoteOctave(int note) { return note / 12 - 1; }

// ── Transport ────────────────────────────────────────────────────────────────

void registerProperties(ecs::transport_control_component& comp, ComponentInspector& inspector)
{
    inspector.addCustomProperty("transport_playback", [&]() {
        // Play button (green)
        ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.15f, 0.55f, 0.15f, 1.f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.20f, 0.70f, 0.20f, 1.f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(0.10f, 0.45f, 0.10f, 1.f));
        if (ImGui::Button(comp.playing && !comp.paused ? "Playing" : "Play")) comp.play();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();

        // Pause button (yellow)
        ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.55f, 0.50f, 0.10f, 1.f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.70f, 0.65f, 0.15f, 1.f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(0.45f, 0.40f, 0.08f, 1.f));
        if (ImGui::Button(comp.paused ? "Resume" : "Pause")) comp.pause();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();

        // Stop button (red)
        ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.55f, 0.15f, 0.15f, 1.f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.70f, 0.20f, 0.20f, 1.f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(0.45f, 0.10f, 0.10f, 1.f));
        if (ImGui::Button("Stop")) comp.stop();
        ImGui::PopStyleColor(3);
    });

    inspector.addProperty("BPM", (float*)&comp.bpm, 20.f, 300.f, 0.5f);

    inspector.addCustomProperty("transport_timesig", [&]() {
        ImGui::SetNextItemWidth(50.f);
        ImGui::DragInt("Num##ts", &comp.timeSigNum, 1, 1, 16);
        ImGui::SameLine();
        ImGui::Text("/");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(50.f);
        const int denChoices[] = {1,2,4,8,16};
        if (ImGui::BeginCombo("Den##ts", std::to_string(comp.timeSigDen).c_str())) {
            for (int d : denChoices) {
                bool sel = (comp.timeSigDen == d);
                if (ImGui::Selectable(std::to_string(d).c_str(), sel))
                    comp.timeSigDen = d;
                if (sel) ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
    });

    inspector.addCustomProperty("transport_position", [&]() {
        ImGui::Text("Bar %llu  Beat %d  %.2f beats",
                    (unsigned long long)comp.barIndex + 1,
                    comp.beatInBar + 1,
                    comp.songPositionBeats);
    });

    inspector.addProperty("Loop", &comp.loopEnabled);
    if (comp.loopEnabled) {
        inspector.addProperty("Loop Start", (float*)&comp.loopStartBeats, 0.f, 999.f, 0.25f);
        inspector.addProperty("Loop End",   (float*)&comp.loopEndBeats,   0.f, 999.f, 0.25f);
    }
}

// ── Clock ─────────────────────────────────────────────────────────────────────

void registerProperties(ecs::clock_component& comp, ComponentInspector& inspector)
{
    inspector.addProperty("BPM", (float*)&comp.bpm, 20.f, 300.f, 0.5f);

    inspector.addCustomProperty("clock_ppq", [&]() {
        const int ppqChoices[] = {24, 48, 96, 192};
        const char* ppqLabels[] = {"24", "48", "96", "192"};
        int cur = 2;
        for (int i = 0; i < 4; ++i) if (ppqChoices[i] == comp.ppq) { cur = i; break; }
        if (ImGui::Combo("PPQ", &cur, ppqLabels, 4))
            comp.ppq = ppqChoices[cur];
    });

    inspector.addProperty("Swing", &comp.swingAmount, 0.f, 1.f, 0.01f);
    inspector.addProperty("External Sync", &comp.externalSync);

    inspector.addCustomProperty("clock_status", [&]() {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.6f, 0.6f, 0.6f, 1.f));
        ImGui::Text("Bar %llu  Beat %d  Tick %d",
                    (unsigned long long)comp.barCount + 1,
                    comp.beatInBar + 1,
                    comp.tickInBeat);
        ImGui::Text("Phase: %.2f ticks", comp.phaseTicks);
        ImGui::PopStyleColor();
    });
}

// ── Sequencer ────────────────────────────────────────────────────────────────

void registerProperties(ecs::sequencer_component& comp, ComponentInspector& inspector)
{
    inspector.addProperty("Steps", &comp.numSteps, 1, 64);
    inspector.addProperty("Playing", &comp.playing);
    inspector.addProperty("Quantized Start", &comp.quantizedStart);

    inspector.addCustomProperty("seq_progress", [&]() {
        float progress = comp.numSteps > 0
            ? (float)(comp.currentStep + 1) / (float)comp.numSteps
            : 0.f;
        char overlay[32];
        snprintf(overlay, sizeof(overlay), "Step %d / %d", comp.currentStep + 1, comp.numSteps);
        ImGui::ProgressBar(progress, ImVec2(-1, 8.f), overlay);
    });

    inspector.addCustomProperty("seq_clock", [&]() {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.6f, 0.6f, 0.6f, 1.f));
        if (comp.clockSource == entt::null)
            ImGui::Text("Clock: none");
        else
            ImGui::Text("Clock entity: %u", (unsigned)comp.clockSource);
        ImGui::PopStyleColor();
    });
}

// ── Pattern ──────────────────────────────────────────────────────────────────

void registerProperties(ecs::pattern_component& comp, ComponentInspector& inspector)
{
    inspector.addProperty("Name", &comp.name);
    inspector.addProperty("Steps", &comp.numSteps, 1, 64);
    inspector.addProperty("BPM",   &comp.bpm, 20.f, 300.f, 0.5f);

    inspector.addCustomProperty("pattern_root", [&]() {
        ImGui::SetNextItemWidth(80.f);
        int n = comp.rootNote;
        if (ImGui::DragInt("Root##root", &n, 1, 0, 127))
            comp.rootNote = n;
        ImGui::SameLine();
        ImGui::TextDisabled("%s%d", midiNoteName(comp.rootNote), midiNoteOctave(comp.rootNote));
    });

    inspector.addCustomProperty("pattern_scale", [&]() {
        const char* scales[] = {"Chromatic","Major","Minor","Pentatonic","Blues"};
        ImGui::Combo("Scale", &comp.scale, scales, 5);
    });

    inspector.addCustomProperty("pattern_rev", [&]() {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f,0.5f,0.5f,1.f));
        ImGui::Text("Rev: %u", comp.revision);
        ImGui::PopStyleColor();
        if (ImGui::Button("Bump revision##pat")) {
            ++comp.revision;
        }
    });
}

// ── Trigger pattern data ─────────────────────────────────────────────────────

void registerProperties(ecs::trigger_pattern_data_component& comp, ComponentInspector& inspector)
{
    inspector.addProperty("Lanes", &comp.numLanes, 1, 9999);
    inspector.addProperty("Steps", &comp.numSteps, 1, 9999);

    inspector.addCustomProperty("trigger_pat_grid", [&]() {
        comp.resizeGrid(comp.numLanes, comp.numSteps);

        int active = 0;
        const int lanes = std::min(comp.numLanes, static_cast<int>(comp.grid.size()));
        const int steps = comp.grid.empty() ? 0 : static_cast<int>(comp.grid[0].size());
        for (int r = 0; r < lanes; ++r) {
            const int rowSteps = std::min(steps, static_cast<int>(comp.grid[r].size()));
            for (int s = 0; s < rowSteps; ++s)
                if (comp.grid[r][s].active) ++active;
        }
        ImGui::TextDisabled("%d active steps  (%d lanes x %d steps)", active, lanes, steps);
        if (ImGui::Button("Apply grid to sequencer##tpd")) {
            comp.resizeGrid(comp.numLanes, comp.numSteps);
            comp.requestApply = true;
        }
        ImGui::SameLine();
        if (ImGui::Button("Clear grid##tpd")) {
            comp.resizeGrid(comp.numLanes, comp.numSteps);
            for (auto& row : comp.grid)
                for (auto& step : row)
                    step = {};
            comp.requestApply = true;
        }
    });
}

// ── MIDI Output ──────────────────────────────────────────────────────────────

void registerProperties(ecs::midi_output_component& comp, ComponentInspector& inspector)
{
    inspector.addProperty("Port", &comp.portName);

    inspector.addCustomProperty("midi_out_status", [&]() {
        ImVec4 col = comp.isOpen
            ? ImVec4(0.2f, 0.8f, 0.2f, 1.f)
            : ImVec4(0.8f, 0.3f, 0.3f, 1.f);
        ImGui::PushStyleColor(ImGuiCol_Text, col);
        ImGui::Text(comp.isOpen ? "Open" : "Closed");
        ImGui::PopStyleColor();
        ImGui::SameLine();
        ImGui::TextDisabled("(%d pending)", (int)comp.pending.size());
    });
}

// ── Trigger lane ─────────────────────────────────────────────────────────────

void registerProperties(ecs::trigger_lane_component& comp, ComponentInspector& inspector)
{
    inspector.addProperty("Name", &comp.name);
    inspector.addProperty("Lane index", &comp.laneIndex, 0, 15);
    inspector.addProperty("Melodic lane", &comp.melodic);
    inspector.addCustomProperty("trigger_lane_note", [&]() {
        ImGui::SetNextItemWidth(80.f);
        int n = comp.defaultNote;
        if (ImGui::DragInt("Default note", &n, 1, -1, 127))
            comp.defaultNote = n;
        ImGui::SameLine();
        if (comp.defaultNote >= 0)
            ImGui::TextDisabled("%s%d", midiNoteName(comp.defaultNote), midiNoteOctave(comp.defaultNote));
        else
            ImGui::TextDisabled("unpitched");
    });
}

// ── Trigger pattern ──────────────────────────────────────────────────────────

void registerProperties(ecs::trigger_pattern_component& comp, ComponentInspector& inspector)
{
    inspector.addProperty("Name", &comp.name);
    inspector.addProperty("Steps", &comp.numSteps, 1, 9999);
    inspector.addProperty("Lanes", &comp.numLanes, 1, 9999);
    inspector.addProperty("BPM", &comp.bpm, 20.f, 300.f, 0.5f);

    inspector.addCustomProperty("trigger_pat_root", [&]() {
        ImGui::SetNextItemWidth(80.f);
        int n = comp.rootNote;
        if (ImGui::DragInt("Root##tproot", &n, 1, 0, 127))
            comp.rootNote = n;
        ImGui::SameLine();
        ImGui::TextDisabled("%s%d", midiNoteName(comp.rootNote), midiNoteOctave(comp.rootNote));
    });

    inspector.addCustomProperty("trigger_pat_scale", [&]() {
        const char* scales[] = {"Chromatic","Major","Minor","Pentatonic","Blues"};
        ImGui::Combo("Scale", &comp.scale, scales, 5);
    });

    inspector.addCustomProperty("trigger_pat_rev", [&]() {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f,0.5f,0.5f,1.f));
        ImGui::Text("Rev: %u", comp.revision);
        ImGui::PopStyleColor();
        if (ImGui::Button("Bump + apply##tpat")) {
            ++comp.revision;
            comp.requestApply = true;
        }
    });
}

// ── Trigger sequencer ────────────────────────────────────────────────────────

void registerProperties(ecs::trigger_sequencer_component& comp, ComponentInspector& inspector)
{
    inspector.addProperty("Steps", &comp.numSteps, 1, 9999);
    inspector.addProperty("Steps / beat", &comp.stepsPerBeat, 1, 64);
    inspector.addProperty("Playing", &comp.playing);
    inspector.addProperty("Chain enabled", &comp.chainEnabled);
    inspector.addProperty("Chain length", &comp.chainLength, 1, 9999);
    inspector.addProperty("Active bank", &comp.activeBank, 0, 9999);

    inspector.addCustomProperty("trigger_seq_progress", [&]() {
        float progress = comp.numSteps > 0
            ? (float)(comp.currentStep + 1) / (float)comp.numSteps
            : 0.f;
        char overlay[32];
        snprintf(overlay, sizeof(overlay), "Step %d / %d", comp.currentStep + 1, comp.numSteps);
        ImGui::ProgressBar(progress, ImVec2(-1, 8.f), overlay);
    });

    inspector.addCustomProperty("trigger_seq_bar", [&]() {
        ImGui::TextDisabled("Bar %d  chain pos %d", comp.currentBar + 1, comp.chainPosition);
        if (comp.clockSource != entt::null)
            ImGui::TextDisabled("Link clock: assign clockSource with externalSync");
    });

    if (ImGui::Button("Reset##trigseq")) {
        comp.reset();
        comp.requestApply = true;
    }

    if (ImGui::Button("Apply config##trigseq"))
        comp.requestApply = true;
}

} // namespace inspector
