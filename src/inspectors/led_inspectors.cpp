// ============================================================================
// LED / UV Component Inspectors - Implementation
// pfd replaced with ImGuiFileDialog (IGFD).
// ============================================================================

#include "led_inspectors.h"
#include "ofxImGuiFileDialog.h"
#include "ofMain.h"

namespace inspector {

void registerProperties(ecs::uv_component& comp, ComponentInspector& inspector) {
    inspector.addProperty("Name",       &comp.name);
    inspector.addProperty("Width",      &comp.width,     1.f, 10000.f);
    inspector.addProperty("Height",     &comp.height,    1.f, 10000.f);
    inspector.addCustomProperty("LED Count",    [&comp]() { ImGui::Text("LEDs: %d", comp.getLedCount()); });
    inspector.addCustomProperty("Show Preview", [&comp]() { ImGui::Checkbox("##ShowPreview", &comp.showPreview); });
    inspector.addProperty("Draw Scale", &comp.drawScale, 0.01f, 10.f);

    inspector.addCustomProperty("Actions", [&comp]() {
        ImVec2 minSz(500, 350), maxSz(900, 700);

        if (ImGui::Button("Load UV Mapping...")) {
            IGFD::FileDialogConfig cfg; cfg.path = ofToDataPath("", true);
            ImGuiFileDialog::Instance()->OpenDialog("UVLoad", "Select UV mapping JSON", ".json", cfg);
        }
        ImGui::SameLine();
        if (ImGui::Button("Save UV Mapping...")) {
            IGFD::FileDialogConfig cfg; cfg.path = ofToDataPath("", true); cfg.fileName = "mapping_uv.json";
            ImGuiFileDialog::Instance()->OpenDialog("UVSave", "Save UV mapping", ".json", cfg);
        }

        if (ImGuiFileDialog::Instance()->Display("UVLoad", ImGuiWindowFlags_NoCollapse, minSz, maxSz)) {
            if (ImGuiFileDialog::Instance()->IsOk()) {
                std::string sel = ImGuiFileDialog::Instance()->GetFilePathName();
                if (comp.loadFromJSON(sel))
                    ofLogNotice("UV Component") << "Loaded " << sel;
            }
            ImGuiFileDialog::Instance()->Close();
        }
        if (ImGuiFileDialog::Instance()->Display("UVSave", ImGuiWindowFlags_NoCollapse, minSz, maxSz)) {
            if (ImGuiFileDialog::Instance()->IsOk()) {
                std::string dest = ImGuiFileDialog::Instance()->GetFilePathName();
                if (comp.saveToJSON(dest))
                    ofLogNotice("UV Component") << "Saved mapping to " << dest;
            }
            ImGuiFileDialog::Instance()->Close();
        }
    });
}

void registerProperties(ecs::uv_component& comp, ComponentInspector& inspector,
                        entt::registry& registry, entt::entity entity) {
    registerProperties(comp, inspector);

    inspector.addCustomProperty("Source Canvas", [&comp, &registry]() {
        std::vector<std::pair<entt::entity, std::string>> items;
        items.push_back({entt::null, "(None)"});
        auto view = registry.view<ecs::fbo_component, ecs::named_entity>();
        for (auto e : view) {
            std::string name = registry.get<ecs::named_entity>(e).getName();
            auto& fc = registry.get<ecs::fbo_component>(e);
            if (fc.fbo.isAllocated())
                name += " (" + std::to_string((int)fc.fbo.getWidth()) + "x" + std::to_string((int)fc.fbo.getHeight()) + ")";
            items.push_back({e, name});
        }
        int cur = 0;
        for (int i = 0; i < (int)items.size(); i++) if (items[i].first == comp.sourceEntity) { cur = i; break; }
        ImGui::PushItemWidth(-1);
        if (ImGui::BeginCombo("##SourceCanvas", items[cur].second.c_str())) {
            for (int i = 0; i < (int)items.size(); i++) {
                bool sel = (items[i].first == comp.sourceEntity);
                if (ImGui::Selectable(items[i].second.c_str(), sel)) comp.sourceEntity = items[i].first;
                if (sel) ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
        ImGui::PopItemWidth();
    });

    inspector.addCustomProperty("Drawable", [&comp, &registry, entity]() {
        std::vector<std::pair<entt::entity, std::string>> items;
        items.push_back({entt::null, "(Default: circle)"});
        for (auto e : registry.view<ecs::named_entity>()) {
            if (e == entity) continue;
            std::string name = registry.get<ecs::named_entity>(e).getName();
            bool mesh  = registry.any_of<ecs::mesh_component>(e);
            bool image = registry.any_of<ecs::image_component>(e);
            if (mesh || image) {
                name += mesh ? " [Mesh]" : " [Image]";
                items.push_back({e, name});
            }
        }
        int cur = 0;
        for (int i = 0; i < (int)items.size(); i++) if (items[i].first == comp.drawableEntity) { cur = i; break; }
        ImGui::PushItemWidth(-1);
        if (ImGui::BeginCombo("##Drawable", items[cur].second.c_str())) {
            for (int i = 0; i < (int)items.size(); i++) {
                bool sel = (items[i].first == comp.drawableEntity);
                if (ImGui::Selectable(items[i].second.c_str(), sel)) comp.drawableEntity = items[i].first;
                if (sel) ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
        ImGui::PopItemWidth();
    });
}

// ============================================================================
// UV Sample Component Inspector
// ============================================================================

void registerProperties(ecs::uv_sample_component& comp, ComponentInspector& inspector) {
    inspector.addProperty("Auto Sample", &comp.autoSample);
}

void registerProperties(ecs::uv_sample_component& comp, ComponentInspector& inspector,
                        entt::registry& registry, entt::entity /*entity*/) {
    registerProperties(comp, inspector);

    inspector.addCustomProperty("UV Source", [&comp, &registry]() {
        std::vector<std::pair<entt::entity, std::string>> uvEntities;
        uvEntities.push_back({entt::null, "(None)"});
        for (auto e : registry.view<ecs::uv_component>()) {
            std::string name = "Entity " + std::to_string(static_cast<uint32_t>(e));
            if (registry.any_of<ecs::named_entity>(e))
                name = registry.get<ecs::named_entity>(e).getName();
            auto& uv = registry.get<ecs::uv_component>(e);
            name += " (" + std::to_string(uv.getLedCount()) + " LEDs)";
            uvEntities.push_back({e, name});
        }
        int cur = 0;
        for (int i = 0; i < (int)uvEntities.size(); i++) if (uvEntities[i].first == comp.uvEntity) { cur = i; break; }
        ImGui::PushItemWidth(200);
        if (ImGui::BeginCombo("##UVSource", uvEntities[cur].second.c_str())) {
            for (int i = 0; i < (int)uvEntities.size(); i++) {
                bool sel = (i == cur);
                if (ImGui::Selectable(uvEntities[i].second.c_str(), sel)) comp.uvEntity = uvEntities[i].first;
                if (sel) ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
        ImGui::PopItemWidth();
    });
    inspector.addCustomProperty("Status", [&comp, &registry]() {
        if (comp.uvEntity == entt::null) ImGui::TextDisabled("No UV source selected");
        else if (!registry.valid(comp.uvEntity)) ImGui::TextColored(ImVec4(1,0.3f,0.3f,1), "Invalid UV entity");
        else ImGui::Text("Sampled: %d colors", comp.getColorCount());
    });
}

} // namespace inspector
