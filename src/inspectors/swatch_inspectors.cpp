#include "swatch_inspectors.h"
#include "ColorTheory.h"
#include "imgui.h"

namespace inspector {

static int s_gradientDraggingStop = -1;
static int s_gradientSelectedStop = -1;
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

void registerProperties(ecs::color_gradient_component& c, ComponentInspector& inspector) {
    inspector.addProperty("Gradient Name", &c.name);
    
    inspector.addCustomProperty("Stop Count", [&c]() {
        ImGui::Text("%d stops", c.count());
    });
    
    inspector.addCustomProperty("Visual Editor", [&c]() {
        ImVec2 cursorPos = ImGui::GetCursorScreenPos();
        float width = ImGui::GetContentRegionAvail().x;
        float gradientHeight = 30.0f;
        float markerHeight = 12.0f;
        float totalHeight = gradientHeight + markerHeight + 4.0f;
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        
        int numStrips = (int)width;
        for (int s = 0; s < numStrips; s++) {
            float t = (float)s / std::max(1, numStrips - 1);
            ofColor col = c.getColorAt(t);
            ImU32 imCol = IM_COL32(col.r, col.g, col.b, col.a);
            drawList->AddRectFilled(
                ImVec2(cursorPos.x + s, cursorPos.y),
                ImVec2(cursorPos.x + s + 1, cursorPos.y + gradientHeight),
                imCol
            );
        }
        
        drawList->AddRect(cursorPos, ImVec2(cursorPos.x + width, cursorPos.y + gradientHeight),
                         IM_COL32(80, 80, 80, 255), 2.0f);
        
        float markerY = cursorPos.y + gradientHeight + 2.0f;
        float markerWidth = 10.0f;
        
        for (int i = 0; i < c.count(); i++) {
            float x = cursorPos.x + c.stops[i].position * width;
            ImVec2 p1(x, markerY);
            ImVec2 p2(x - markerWidth * 0.5f, markerY + markerHeight);
            ImVec2 p3(x + markerWidth * 0.5f, markerY + markerHeight);
            
            ofColor& stopColor = c.stops[i].color.color;
            ImU32 fillColor = IM_COL32(stopColor.r, stopColor.g, stopColor.b, 255);
            drawList->AddTriangleFilled(p1, p2, p3, fillColor);
            
            bool isSelected = (s_gradientSelectedStop == i);
            ImU32 outlineColor = isSelected ? IM_COL32(255, 255, 255, 255) : IM_COL32(60, 60, 60, 255);
            float outlineThickness = isSelected ? 2.0f : 1.0f;
            drawList->AddTriangle(p1, p2, p3, outlineColor, outlineThickness);
            
            drawList->AddLine(
                ImVec2(x, cursorPos.y + 1),
                ImVec2(x, cursorPos.y + gradientHeight - 1),
                IM_COL32(255, 255, 255, 80), 1.0f
            );
        }
        
        ImGui::SetCursorScreenPos(cursorPos);
        ImGui::InvisibleButton("gradientEditor", ImVec2(width, totalHeight));
        
        if (ImGui::IsItemHovered() || ImGui::IsItemActive()) {
            ImVec2 mousePos = ImGui::GetMousePos();
            float mouseX = mousePos.x - cursorPos.x;
            float mouseY = mousePos.y - cursorPos.y;
            bool inMarkerArea = mouseY > gradientHeight;
            
            int closestStop = -1;
            float closestDist = markerWidth;
            for (int i = 0; i < c.count(); i++) {
                float stopX = c.stops[i].position * width;
                float dist = std::abs(mouseX - stopX);
                if (dist < closestDist) {
                    closestDist = dist;
                    closestStop = i;
                }
            }
            
            if (ImGui::IsMouseClicked(0)) {
                if (inMarkerArea && closestStop >= 0) {
                    s_gradientSelectedStop = closestStop;
                    s_gradientDraggingStop = closestStop;
                } else if (!inMarkerArea) {
                    float newPos = ofClamp(mouseX / width, 0.0f, 1.0f);
                    ofColor newColor = c.getColorAt(newPos);
                    c.addStop(newPos, newColor);
                    for (int i = 0; i < c.count(); i++) {
                        if (std::abs(c.stops[i].position - newPos) < 0.001f) {
                            s_gradientSelectedStop = i;
                            s_gradientDraggingStop = i;
                            break;
                        }
                    }
                }
            }
            
            if (s_gradientDraggingStop >= 0 && ImGui::IsMouseDragging(0)) {
                float newPos = ofClamp(mouseX / width, 0.0f, 1.0f);
                c.stops[s_gradientDraggingStop].position = newPos;
            }
            
            if (ImGui::IsMouseReleased(0)) {
                if (s_gradientDraggingStop >= 0) {
                    c.sortStops();
                    s_gradientDraggingStop = -1;
                }
            }
            
            if (ImGui::IsMouseDoubleClicked(0) && inMarkerArea && closestStop >= 0 && c.count() > 2) {
                c.removeStop(closestStop);
                s_gradientSelectedStop = -1;
                s_gradientDraggingStop = -1;
            }
            
            if (closestStop >= 0 && inMarkerArea) {
                ImGui::BeginTooltip();
                ImGui::Text("Position: %.2f", c.stops[closestStop].position);
                ImGui::Text("Double-click to delete");
                ImGui::EndTooltip();
            } else if (!inMarkerArea) {
                ImGui::BeginTooltip();
                ImGui::Text("Click to add stop");
                ImGui::EndTooltip();
            }
        }
        
        ImGui::SetCursorScreenPos(ImVec2(cursorPos.x, cursorPos.y + totalHeight + 4.0f));
    });
    
    inspector.addCustomProperty("Selected Stop", [&c]() {
        if (s_gradientSelectedStop >= 0 && s_gradientSelectedStop < c.count()) {
            int i = s_gradientSelectedStop;
            ImGui::Text("Stop %d", i + 1);
            
            ImGui::SetNextItemWidth(100);
            float pos = c.stops[i].position;
            if (ImGui::SliderFloat("Position", &pos, 0.0f, 1.0f, "%.3f")) {
                c.stops[i].position = pos;
            }
            if (ImGui::IsItemDeactivatedAfterEdit()) {
                c.sortStops();
            }
            
            float col[4] = {
                c.stops[i].color.color.r / 255.0f,
                c.stops[i].color.color.g / 255.0f,
                c.stops[i].color.color.b / 255.0f,
                c.stops[i].color.color.a / 255.0f
            };
            if (ImGui::ColorEdit4("Color", col, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_DisplayRGB)) {
                c.stops[i].color.color.set(col[0] * 255, col[1] * 255, col[2] * 255, col[3] * 255);
            }
            
            if (!c.stops[i].color.name.empty()) {
                ImGui::Text("Name: %s", c.stops[i].color.name.c_str());
            }
            
            if (c.count() > 2) {
                if (ImGui::Button("Delete Stop")) {
                    c.removeStop(i);
                    s_gradientSelectedStop = -1;
                }
            }
        } else {
            ImGui::TextDisabled("Click a marker to select");
        }
    });
    
    inspector.addCustomProperty("All Stops", [&c]() {
        if (ImGui::TreeNode("Stop List")) {
            for (int i = 0; i < c.count(); i++) {
                ImGui::PushID(i);
                
                bool isSelected = (s_gradientSelectedStop == i);
                if (isSelected) {
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.5f, 0.8f, 1.0f));
                }
                
                ImVec4 color = ImVec4(c.stops[i].color.color.r/255.0f, c.stops[i].color.color.g/255.0f,
                                     c.stops[i].color.color.b/255.0f, c.stops[i].color.color.a/255.0f);
                if (ImGui::ColorButton("##sel", color, ImGuiColorEditFlags_NoTooltip, ImVec2(16, 16))) {
                    s_gradientSelectedStop = i;
                }
                
                if (isSelected) {
                    ImGui::PopStyleColor();
                }
                
                ImGui::SameLine();
                ImGui::Text("%.2f", c.stops[i].position);
                
                ImGui::SameLine();
                if (ImGui::Button("X") && c.count() > 2) {
                    c.removeStop(i);
                    if (s_gradientSelectedStop == i) s_gradientSelectedStop = -1;
                    else if (s_gradientSelectedStop > i) s_gradientSelectedStop--;
                }
                
                ImGui::PopID();
            }
            ImGui::TreePop();
        }
    });
    
    inspector.addCustomProperty("Actions", [&c]() {
        if (ImGui::Button("Add Stop")) {
            c.addStop(0.5f, ofColor::gray);
        }
        ImGui::SameLine();
        if (ImGui::Button("Reverse")) {
            c.reverse();
            s_gradientSelectedStop = -1;
        }
        ImGui::SameLine();
        if (ImGui::Button("Distribute")) {
            int count = c.count();
            for (int i = 0; i < count; i++) {
                c.stops[i].position = (float)i / (count - 1);
            }
            c.sortStops();
        }
        
        if (ImGui::BeginCombo("Presets", "Load Preset...")) {
            if (ImGui::Selectable("Black to White")) {
                c.stops.clear();
                c.addStop(0.0f, ofColor::black);
                c.addStop(1.0f, ofColor::white);
                s_gradientSelectedStop = -1;
            }
            if (ImGui::Selectable("Rainbow")) {
                c.stops.clear();
                c.addStop(0.0f, ofColor::red);
                c.addStop(0.17f, ofColor::orange);
                c.addStop(0.33f, ofColor::yellow);
                c.addStop(0.5f, ofColor::green);
                c.addStop(0.67f, ofColor::cyan);
                c.addStop(0.83f, ofColor::blue);
                c.addStop(1.0f, ofColor::magenta);
                s_gradientSelectedStop = -1;
            }
            if (ImGui::Selectable("Sunset")) {
                c.stops.clear();
                c.addStop(0.0f, ofColor(255, 100, 50));
                c.addStop(0.3f, ofColor(255, 150, 80));
                c.addStop(0.6f, ofColor(200, 100, 150));
                c.addStop(1.0f, ofColor(50, 50, 100));
                s_gradientSelectedStop = -1;
            }
            if (ImGui::Selectable("Ocean")) {
                c.stops.clear();
                c.addStop(0.0f, ofColor(0, 50, 100));
                c.addStop(0.5f, ofColor(0, 150, 200));
                c.addStop(1.0f, ofColor(150, 220, 255));
                s_gradientSelectedStop = -1;
            }
            if (ImGui::Selectable("Fire")) {
                c.stops.clear();
                c.addStop(0.0f, ofColor(30, 0, 0));
                c.addStop(0.3f, ofColor(200, 50, 0));
                c.addStop(0.6f, ofColor(255, 150, 0));
                c.addStop(1.0f, ofColor(255, 255, 200));
                s_gradientSelectedStop = -1;
            }
            ImGui::EndCombo();
        }
    });
}

} // namespace inspector
