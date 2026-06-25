#include "paint_inspectors.h"

#include "entity_inspector.h"                  // inspector::addExtraEntityInspector
#include "ComponentInspector.h"
#include "widgets/ImGradientHDR.h"

#include "components/paint_components.h"        // ecs paint model (glm::vec4)
#include "component_editor_registration.h"     // ecs::registerComponent

#include "imgui.h"

#include <algorithm>
#include <array>

namespace inspector {
namespace {

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
            state.AddColorMarker(g.stops[i].position, rgb, 1.f);
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
            g.stops.emplace_back(m.Position, glm::vec4(m.Color[0], m.Color[1], m.Color[2], a));
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
        if (ImGui::Combo("##gradInterp", &t, "RGB\0HSV\0")) g.interp = (ecs::GradientInterpolation)t;
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

void drawPaintRef(entt::registry& reg, entt::entity paint) {
    ImGui::Text("Paint entity: %u", (unsigned)paint);
    glm::vec4 c;
    if (ecs::resolvePaintColor(reg, paint, c)) {
        ImGui::SameLine();
        ImGui::ColorButton("##paintPreview", ImVec4(c.x, c.y, c.z, c.w));
    }
}

bool inspectFill(entt::registry& reg, entt::entity e) {
    auto& f = reg.get<ecs::fill_component>(e);
    ComponentInspector ci("Fill");
    ci.addCustomProperty("Paint", [&reg, &f]() {
        drawPaintRef(reg, f.paint);
        if (ImGui::SmallButton("Clear##fill")) f.paint = entt::null;
    });
    return ci.hasProperties() ? ci.draw() : false;
}

bool inspectStroke(entt::registry& reg, entt::entity e) {
    auto& s = reg.get<ecs::stroke_component>(e);
    ComponentInspector ci("Stroke");
    ci.addProperty("Width", &s.width, 0.0f, 100.0f, 0.1f);
    ci.addCustomProperty("Paint", [&reg, &s]() {
        drawPaintRef(reg, s.paint);
        if (ImGui::SmallButton("Clear##stroke")) s.paint = entt::null;
    });
    return ci.hasProperties() ? ci.draw() : false;
}

} // namespace

void registerPaintInspectors() {
    static bool done = false;
    if (done) return;
    done = true;

    // Inspector rows (additive — does not clobber the app-level single hook).
    addExtraEntityInspector([](entt::registry& reg, entt::entity e) {
        bool changed = false;
        if (reg.any_of<ecs::solid_color_component>(e))
            changed |= inspectSolidColor(reg.get<ecs::solid_color_component>(e));
        if (reg.any_of<ecs::gradient_component>(e))
            changed |= inspectGradient(reg.get<ecs::gradient_component>(e));
        if (reg.any_of<ecs::fill_component>(e))
            changed |= inspectFill(reg, e);
        if (reg.any_of<ecs::stroke_component>(e))
            changed |= inspectStroke(reg, e);
        return changed;
    });

    // "Add Component" picker rows.
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
