#include "code_snippet_inspectors.h"
#include "ofMain.h"
#include "ofxImGuiTextEdit.h"
#include <algorithm>
#include <map>

namespace inspector {

void registerProperties(ecs::code_snippet_component& comp, ComponentInspector& inspector)
{
    inspector.addCustomProperty("Source", [&comp]() {
        static std::map<void*, TextEditor> editors;
        auto& ed = editors[&comp];

        if (ed.GetText().empty() && !comp.text.empty())
            ed.SetText(comp.text);

        switch (comp.language) {
            case ecs::code_language::Gcode:
                ed.SetLanguageDefinition(TextEditor::LanguageDefinitionId::Gcode);
                break;
            default:
                ed.SetLanguageDefinition(TextEditor::LanguageDefinitionId::None);
                break;
        }

        const float h = ImGui::GetContentRegionAvail().y;
        const float editorH = std::clamp(h > 80.f ? h - 24.f : 220.f, 120.f, 400.f);

        ed.SetReadOnlyEnabled(comp.readOnly);
        if (ed.Render("##embedded_code", true, ImVec2(-1.f, editorH), true))
            comp.text = ed.GetText();
        ImGui::TextDisabled("%d lines", std::max(1, ed.GetLineCount()));
    });
}

} // namespace inspector
