// ============================================================================
// Modulator Component Inspectors - Implementation
// ============================================================================

#include "modulator_inspectors.h"

namespace inspector {

// ============================================================================
// Mod Binding Inspector
// ============================================================================

void registerProperties(ecs::mod_binding_component& binding, ComponentInspector& inspector) {
    inspector.addProperty("Depth", &binding.depth, 0.0f, 1.0f);
    inspector.addProperty("Min", &binding.min);
    inspector.addProperty("Max", &binding.max);
    inspector.addProperty("Additive", &binding.additive);
    
    inspector.addCustomProperty("Status", [&binding]() {
        if (binding.isValid()) {
            ImGui::TextColored(ImVec4(0, 1, 0, 1), "Bound");
            ImGui::SameLine();
            ImGui::Text("Base: %.2f", binding.baseValue);
        } else {
            ImGui::TextColored(ImVec4(1, 0.5f, 0, 1), "Unbound");
        }
    });
}

// ============================================================================
// Modulator Component Inspector
// ============================================================================

void registerProperties(ecs::modulator_component& comp, ComponentInspector& inspector) {
    std::vector<std::string> waveTypes = {"Sine", "Square", "Triangle", "Saw", "Ramp", "Noise", "Smooth Noise", "S&H"};
    inspector.addEnumProperty("Wave Type", (int*)&comp.waveType, waveTypes);
    
    inspector.addProperty("Frequency", &comp.frequency, 0.01f, 100.0f);
    inspector.addProperty("Phase", &comp.phase, 0.0f, 1.0f);
    inspector.addProperty("Playing", &comp.playing);
    
    inspector.addCustomProperty("Controls", [&comp]() {
        if (ImGui::Button("Play")) comp.play();
        ImGui::SameLine();
        if (ImGui::Button("Pause")) comp.pause();
        ImGui::SameLine();
        if (ImGui::Button("Reset")) comp.reset();
    });
    
    inspector.addCustomProperty("Output", [&comp]() {
        ImGui::Text("Value: %.3f", comp.value);
        ImGui::ProgressBar(comp.value);
    });
    
    inspector.addCustomProperty("Waveform Preview", [&comp]() {
        ImVec2 size(200, 60);
        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        
        drawList->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), 
                                IM_COL32(30, 30, 30, 255));
        
        float centerY = pos.y + size.y * 0.5f;
        drawList->AddLine(ImVec2(pos.x, centerY), ImVec2(pos.x + size.x, centerY), 
                         IM_COL32(60, 60, 60, 255));
        
        float prevY = 0;
        for (int i = 0; i < (int)size.x; i++) {
            float t = (float)i / size.x;
            float v = 0.0f;
            
            switch (comp.waveType) {
                case ecs::MOD_SIN:
                    v = (sin(t * TWO_PI) + 1.0f) * 0.5f;
                    break;
                case ecs::MOD_SQUARE:
                    v = t < 0.5f ? 1.0f : 0.0f;
                    break;
                case ecs::MOD_TRIANGLE:
                    v = t < 0.5f ? (t * 2.0f) : (2.0f - t * 2.0f);
                    break;
                case ecs::MOD_SAW:
                    v = t;
                    break;
                case ecs::MOD_RAMP:
                    v = 1.0f - t;
                    break;
                case ecs::MOD_NOISE:
                case ecs::MOD_SMOOTH_NOISE:
                case ecs::MOD_SH:
                    v = ((int)(t * 8) % 2) ? 0.7f : 0.3f;
                    break;
                case ecs::MOD_COUNT:
                    break;
            }
            
            float y = pos.y + size.y - (v * size.y);
            
            if (i > 0) {
                drawList->AddLine(
                    ImVec2(pos.x + i - 1, prevY),
                    ImVec2(pos.x + i, y),
                    IM_COL32(100, 200, 100, 255)
                );
            }
            prevY = y;
        }
        
        float currentX = pos.x + comp.phase * size.x;
        drawList->AddLine(
            ImVec2(currentX, pos.y),
            ImVec2(currentX, pos.y + size.y),
            IM_COL32(255, 100, 100, 255)
        );
        
        ImGui::Dummy(size);
    });
}

} // namespace inspector
