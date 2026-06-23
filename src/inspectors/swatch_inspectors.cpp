#include "swatch_inspectors.h"
#include "ColorTheory.h"
#include "imgui.h"

namespace inspector {

static int s_librarySelectedSwatch = -1;

void registerProperties(ecs::swatch_library_component& c, ComponentInspector& inspector) {
    inspector.addProperty("Library Name", &c.libraryName);
    
    inspector.addCustomProperty("Color Count", [&c]() {
        ImGui::Text("%d colors", c.count());
    });
    
    inspector.addCustomProperty("Colors", [&c]() {
        if (c.empty()) {
            ImGui::TextDisabled("No colors");
        } else {
            float buttonSize = 24.0f;
            float spacing = 2.0f;
            float windowWidth = ImGui::GetContentRegionAvail().x;
            int columns = std::max(1, (int)((windowWidth + spacing) / (buttonSize + spacing)));
            
            int col = 0;
            for (int i = 0; i < c.count(); i++) {
                if (col > 0) ImGui::SameLine();
                
                ImGui::PushID(i);
                if (s_librarySelectedSwatch == i) {
                    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 2.0f);
                }
                ImVec4 color = ImVec4(c.colors[i].color.r/255.0f, c.colors[i].color.g/255.0f,
                                     c.colors[i].color.b/255.0f, c.colors[i].color.a/255.0f);
                ImGuiColorEditFlags flags = ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_AlphaPreview;
                if (ImGui::ColorButton("##color", color, flags, ImVec2(buttonSize, buttonSize))) {
                    s_librarySelectedSwatch = i;
                }
                if (s_librarySelectedSwatch == i) {
                    ImGui::PopStyleVar();
                }
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("%s\nGrey: %.1f%%\nR:%d G:%d B:%d", 
                        c.colors[i].getDisplayName().c_str(),
                        c.colors[i].getGreyValuePercent(),
                        (int)c.colors[i].color.r, (int)c.colors[i].color.g, (int)c.colors[i].color.b);
                }
                ImGui::PopID();
                
                col++;
                if (col >= columns) col = 0;
            }
        }
    });

    inspector.addCustomProperty("Selected Swatch", [&c]() {
        if (s_librarySelectedSwatch < 0 || s_librarySelectedSwatch >= c.count()) {
            ImGui::TextDisabled("Select a swatch above");
            return;
        }
        auto& sw = c.colors[s_librarySelectedSwatch];
        char nameBuf[128];
        strncpy(nameBuf, sw.name.c_str(), sizeof(nameBuf) - 1);
        nameBuf[sizeof(nameBuf) - 1] = '\0';
        if (ImGui::InputText("Name", nameBuf, sizeof(nameBuf))) {
            sw.name = nameBuf;
        }
        ImGui::Text("Grey value: %.1f%%", sw.getGreyValuePercent());
        if (sw.isNeutralGrey()) ImGui::TextDisabled("Neutral grey (R=G=B)");

        float col[4] = { sw.color.r/255.f, sw.color.g/255.f, sw.color.b/255.f, sw.color.a/255.f };
        if (ImGui::ColorEdit4("RGB", col, ImGuiColorEditFlags_AlphaBar)) {
            sw.color.set(col[0]*255, col[1]*255, col[2]*255, col[3]*255);
            if (sw.type == ofxSwatches::SwatchColorType::CMYK) {
                sw.cmyk100 = ofxSwatches::SwatchColor::rgbToCmyk(sw.color, &c.richBlack);
            }
        }

        if (ImGui::Button("Remove selected")) {
            c.removeColor(s_librarySelectedSwatch);
            s_librarySelectedSwatch = -1;
        }
    });
    
    inspector.addCustomProperty("Actions", [&c]() {
        if (ImGui::Button("Add RGB")) {
            c.addColor(ofColor::white, "New");
        }
        ImGui::SameLine();
        if (ImGui::Button("Clear All")) {
            c.clear();
            s_librarySelectedSwatch = -1;
        }
    });
}

void registerProperties(ecs::swatch_palette_ref_component& c, ComponentInspector& inspector) {
    inspector.addCustomProperty("Palette", [&c]() {
        ImGui::Text("Library entity: %u", (unsigned)c.library);
        ImGui::Text("Color index: %d", c.colorIndex);
        if (!c.colorName.empty()) {
            ImGui::Text("Color name: %s", c.colorName.c_str());
        }
    });
}

} // namespace inspector
