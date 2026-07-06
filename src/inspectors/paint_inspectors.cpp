#include "paint_inspectors.h"

#include "entity_inspector.h"                  // inspector::addExtraEntityInspector
#include "ComponentInspector.h"
#include "widgets/ImGradientHDR.h"

#include "components/paint_components.h"        // ecs paint model (glm::vec4)
#include "component_editor_registration.h"     // ecs::registerComponent

#include "imgui.h"

#include <algorithm>
#include <array>
#include <string>
#include <vector>

namespace inspector {
namespace {

entt::entity findPaintLibraryHost(entt::registry& reg)
{
	auto view = reg.view<ecs::paint_library_component>();
	if (view.begin() == view.end())
		return entt::null;
	return *view.begin();
}

entt::entity createPaintSolid(entt::registry& reg, const glm::vec4& color)
{
	const entt::entity host = findPaintLibraryHost(reg);
	if (host != entt::null)
		return ecs::createLibrarySolidPaint(reg, host, color);
	return ecs::createSolidPaint(reg, color);
}

entt::entity createPaintGradient(entt::registry& reg, const std::string& name)
{
	const entt::entity host = findPaintLibraryHost(reg);
	if (host != entt::null)
		return ecs::createLibraryGradientPaint(reg, host, name);
	return ecs::createGradientPaint(reg, name);
}

std::string paintLabel(entt::registry& reg, entt::entity paint)
{
    if (paint == entt::null || !reg.valid(paint)) return "(none)";
    if (reg.any_of<ecs::solid_color_component>(paint)) return "Solid";
    if (const auto* g = reg.try_get<ecs::gradient_component>(paint))
        return g->name.empty() ? "Gradient" : g->name;
    return "Paint";
}

void drawPaintRef(entt::registry& reg, entt::entity paint) {
    ImGui::Text("Paint entity: %u", (unsigned)paint);
    if (paint != entt::null && reg.valid(paint)
        && reg.any_of<ecs::solid_color_component>(paint)) {
        auto& sc = reg.get<ecs::solid_color_component>(paint);
        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::ColorEdit4("##paintColor", &sc.color.x, ImGuiColorEditFlags_AlphaBar);
    } else {
        glm::vec4 c;
        if (ecs::resolvePaintColor(reg, paint, c)) {
            ImGui::SameLine();
            ImGui::ColorButton("##paintPreview", ImVec4(c.x, c.y, c.z, c.w));
        }
    }
}

void paintSelectable(entt::registry& reg, entt::entity pe, entt::entity current,
                     entt::entity& chosen, const char* fallbackLabel)
{
    const bool sel = pe == current;
    ImGui::PushID(static_cast<int>(pe));
    const std::string label = paintLabel(reg, pe);
    if (ImGui::Selectable(label == "Paint" ? fallbackLabel : label.c_str(), sel))
        chosen = pe;
    if (reg.any_of<ecs::solid_color_component>(pe)) {
        const auto& c = reg.get<ecs::solid_color_component>(pe).color;
        ImGui::SameLine();
        ImGui::ColorButton("##sw", ImVec4(c.x, c.y, c.z, c.w));
    }
    ImGui::PopID();
}

entt::entity pickPaintEntity(entt::registry& reg, entt::entity current)
{
    entt::entity chosen = current;
    if (ImGui::BeginCombo("##paintPick", paintLabel(reg, current).c_str())) {
        if (ImGui::Selectable("(none)", current == entt::null))
            chosen = entt::null;

        const entt::entity host = findPaintLibraryHost(reg);
        if (host != entt::null)
            ecs::prunePaintLibrary(reg, host);
        const auto* docPaints = host != entt::null
            ? reg.try_get<ecs::paint_library_component>(host)
            : nullptr;

        if (docPaints && !docPaints->paints.empty()) {
            ImGui::Separator();
            ImGui::TextDisabled("Document paints");
            for (entt::entity pe : docPaints->paints) {
                if (!reg.valid(pe)) continue;
                paintSelectable(reg, pe, current, chosen, "Document paint");
            }
        }

        ImGui::Separator();
        ImGui::TextDisabled("All paints");
        for (auto [pe, sc] : reg.view<ecs::solid_color_component>().each()) {
            (void)sc;
            paintSelectable(reg, pe, current, chosen, "Solid##paint");
        }
        for (auto [pe, gc] : reg.view<ecs::gradient_component>().each()) {
            (void)gc;
            paintSelectable(reg, pe, current, chosen, "Gradient##paint");
        }
        ImGui::EndCombo();
    }
    return chosen;
}

void ensureSolidPaint(entt::registry& reg, entt::entity& paintRef)
{
    if (paintRef != entt::null && reg.valid(paintRef)
        && reg.any_of<ecs::solid_color_component>(paintRef))
        return;
    paintRef = createPaintSolid(reg, glm::vec4(1.f));
}

void drawPaintRefEditor(entt::registry& reg, entt::entity& paintRef)
{
    if (paintRef == entt::null)
        ensureSolidPaint(reg, paintRef);

    drawPaintRef(reg, paintRef);
    if (ImGui::SmallButton("New solid##paint")) {
        paintRef = createPaintSolid(reg, glm::vec4(1.f));
    }
    ImGui::SameLine();
    if (ImGui::SmallButton("New gradient##paint")) {
        paintRef = createPaintGradient(reg, "Gradient");
    }
    ImGui::SameLine();
    if (ImGui::SmallButton("Clear##paintClr")) paintRef = entt::null;
    paintRef = pickPaintEntity(reg, paintRef);
}

// ---------------------------------------------------------------------------
// Gradient stop editor (ImGradientHDR <-> ecs::gradient_component)
// ---------------------------------------------------------------------------

void drawGradientStops(ecs::gradient_component& g) {
    static ImGradientHDRState          state;
    static ImGradientHDRTemporaryState temp;
    static const void*                 lastKey = nullptr;

    const void* key = static_cast<const void*>(&g);
    if (lastKey != key) {
        state = ImGradientHDRState{};
        int n = std::min((int)g.stops.size(), (int)MarkerMax);
        for (int i = 0; i < n; ++i) {
            const glm::vec4& c = g.stops[i].color;
            std::array<float, 3> rgb = { c.x, c.y, c.z };
            state.AddColorMarker(g.stops[i].position, rgb, g.stops[i].intensity);
            state.AddAlphaMarker(g.stops[i].position, c.w);
        }
        if (state.ColorCount == 0) {
            state.AddColorMarker(0.f, { 0.f, 0.f, 0.f }, 1.f);
            state.AddColorMarker(1.f, { 1.f, 1.f, 1.f }, 1.f);
        }
        if (state.AlphaCount == 0) {
            state.AddAlphaMarker(0.f, 1.f);
            state.AddAlphaMarker(1.f, 1.f);
        }
        lastKey = key;
    }

    if (ImGradientHDR(ImGui::GetID("paintGradHdr"), state, temp)) {
        g.stops.clear();
        for (int i = 0; i < state.ColorCount; ++i) {
            const auto& m = state.Colors[i];
            float a = state.GetAlpha(m.Position);
            g.stops.emplace_back(m.Position,
                                 glm::vec4(m.Color[0], m.Color[1], m.Color[2], a),
                                 m.Intensity);
        }
        g.sortStops();
    }
}

bool inspectGradient(ecs::gradient_component& g) {
    ComponentInspector ci("Gradient");
    ci.addProperty("Name", &g.name);

    ci.addCustomProperty("Type", [&g]() {
        int t = (int)g.type;
        if (ImGui::Combo("##gradType", &t, "Linear\0Radial\0")) g.type = (ecs::GradientType)t;
    });
    ci.addCustomProperty("Interpolation", [&g]() {
        int t = (int)g.interp;
        if (ImGui::Combo("##gradInterp", &t, "RGB\0HSV\0OkLab\0"))
            g.interp = (ecs::GradientInterpolation)t;
    });
    ci.addCustomProperty("Spread", [&g]() {
        int t = (int)g.spread;
        if (ImGui::Combo("##gradSpread", &t, "Pad\0Repeat\0Mirror\0")) g.spread = (ecs::GradientSpread)t;
    });

    if (g.type == ecs::GradientType::Linear) {
        ci.addCustomProperty("Angle", [&g]() {
            ImGui::DragFloat("##gradAngle", &g.angle, 1.0f, 0.0f, 360.0f, "%.0f");
        });
    } else {
        ci.addCustomProperty("Radial", [&g]() {
            ImGui::DragFloat2("Center", &g.center.x, 0.01f, 0.0f, 1.0f);
            ImGui::DragFloat("Inner Radius", &g.innerRadius, 0.01f, 0.0f, 4.0f);
            ImGui::DragFloat("Outer Radius", &g.outerRadius, 0.01f, 0.0f, 4.0f);
        });
    }

    ci.addCustomProperty("Steps", [&g]() {
        ImGui::DragInt("##gradSteps", &g.numSteps, 1, 0, 512);
        ImGui::SameLine();
        ImGui::TextDisabled("0 = full resolution");
    });

    ci.addCustomProperty("Stops", [&g]() { drawGradientStops(g); });

    return ci.hasProperties() ? ci.draw() : false;
}

bool inspectSolidColor(ecs::solid_color_component& s) {
    ComponentInspector ci("Solid Color");
    ci.addCustomProperty("Color", [&s]() { ImGui::ColorEdit4("##solidColor", &s.color.x); });
    return ci.hasProperties() ? ci.draw() : false;
}

bool inspectFill(entt::registry& reg, entt::entity e) {
    auto& f = reg.get<ecs::fill_component>(e);
    ComponentInspector ci("Fill");
    ci.addCustomProperty("Paint", [&reg, &f]() {
        drawPaintRefEditor(reg, f.paint);
    });
    return ci.hasProperties() ? ci.draw() : false;
}

bool inspectStroke(entt::registry& reg, entt::entity e) {
    auto& s = reg.get<ecs::stroke_component>(e);
    ComponentInspector ci("Stroke");
    ci.addProperty("Width", &s.width, 0.0f, 100.0f, 0.1f);
    ci.addCustomProperty("Paint", [&reg, &s]() {
        drawPaintRefEditor(reg, s.paint);
    });
    return ci.hasProperties() ? ci.draw() : false;
}

bool inspectDocumentPaints(entt::registry& reg, entt::entity e) {
    auto* lib = reg.try_get<ecs::paint_library_component>(e);
    if (!lib) return false;

    ecs::prunePaintLibrary(reg, e);

    ComponentInspector ci("Document Paints");
    ci.addCustomProperty("Library", [&reg, e, lib]() {
        if (ImGui::SmallButton("New solid##docPaint"))
            ecs::createLibrarySolidPaint(reg, e, glm::vec4(1.f));
        ImGui::SameLine();
        if (ImGui::SmallButton("New gradient##docPaint"))
            ecs::createLibraryGradientPaint(reg, e, "Gradient");

        for (size_t i = 0; i < lib->paints.size(); ++i) {
            entt::entity pe = lib->paints[i];
            if (!reg.valid(pe)) continue;
            ImGui::PushID(static_cast<int>(pe));
            ImGui::TextUnformatted(paintLabel(reg, pe).c_str());
            ImGui::SameLine();
            if (reg.any_of<ecs::solid_color_component>(pe)) {
                const auto& c = reg.get<ecs::solid_color_component>(pe).color;
                ImGui::ColorButton("##dp", ImVec4(c.x, c.y, c.z, c.w));
                ImGui::SameLine();
            }
            if (ImGui::SmallButton("Remove##docPaint")) {
                ecs::removeLibraryPaint(reg, e, pe);
            }
            ImGui::PopID();
        }
        if (lib->paints.empty())
            ImGui::TextDisabled("No document paints yet");
    });
    return ci.hasProperties() ? ci.draw() : false;
}

} // namespace

void registerPaintInspectors() {
    static bool done = false;
    if (done) return;
    done = true;

    addExtraEntityInspector([](entt::registry& reg, entt::entity e) {
        bool changed = false;
        changed |= inspectDocumentPaints(reg, e);
        if (reg.any_of<ecs::solid_color_component>(e))
            changed |= inspectSolidColor(reg.get<ecs::solid_color_component>(e));
        if (reg.any_of<ecs::gradient_component>(e)) {
            const bool gradChanged = inspectGradient(reg.get<ecs::gradient_component>(e));
            if (gradChanged) ecs::invalidateGradientMeshCache(reg, e);
            changed |= gradChanged;
        }
        if (reg.any_of<ecs::fill_component>(e))
            changed |= inspectFill(reg, e);
        if (reg.any_of<ecs::stroke_component>(e))
            changed |= inspectStroke(reg, e);
        return changed;
    });

    ecs::registerComponent<ecs::solid_color_component>("Solid Color", "Color");
    ecs::registerComponent<ecs::gradient_component>("Gradient", "Color");
    ecs::registerComponent<ecs::fill_component>("Fill", "Color");
    ecs::registerComponent<ecs::stroke_component>("Stroke", "Color");
}

namespace {
struct PaintAutoInit {
    PaintAutoInit() { registerPaintInspectors(); }
};
PaintAutoInit s_paintAutoInit;
} // namespace

} // namespace inspector
