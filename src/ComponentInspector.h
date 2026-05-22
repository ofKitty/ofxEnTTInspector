#pragma once

#include "ofMain.h"
#include "imgui.h"
#include "PropertyReflector.h"
#include <algorithm>
#include <string>
#include <functional>
#include <vector>
#include <filesystem>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace inspector {

struct PropertyInfo {
    std::string          name;
    std::function<bool()> drawImGui;
};

class ComponentInspector {
public:
    ComponentInspector(const std::string& componentName)
        : m_componentName(componentName) {}

    // ── Typed addProperty overloads (UI + reflection) ────────────────────────

    void addProperty(const std::string& name, float* value,
                     float min = 0.f, float max = 100.f, float speed = 1.f) {
        m_properties.push_back({name, [=]() -> bool {
            return ImGui::DragFloat("##", value, speed, min, max);
        }});
        m_reflected.emplace_back(name, PinDataType::Float, value, min, max);
    }

    void addProperty(const std::string& name, int* value,
                     int min = 0, int max = 100, int speed = 1) {
        m_properties.push_back({name, [=]() -> bool {
            return ImGui::DragInt("##", value, (float)speed, min, max);
        }});
        m_reflected.emplace_back(name, PinDataType::Int, value, (float)min, (float)max);
    }

    void addProperty(const std::string& name, bool* value) {
        m_properties.push_back({name, [=]() -> bool {
            return ImGui::Checkbox("##", value);
        }});
        m_reflected.emplace_back(name, PinDataType::Bool, value, 0.f, 1.f);
    }

    void addProperty(const std::string& name, std::string* value) {
        m_properties.push_back({name, [=]() -> bool {
            char buf[256];
            strncpy(buf, value->c_str(), sizeof(buf));
            if (ImGui::InputText("##", buf, sizeof(buf))) {
                *value = buf;
                return true;
            }
            return false;
        }});
        m_reflected.emplace_back(name, PinDataType::String, value);
    }

    void addProperty(const std::string& name, std::filesystem::path* value) {
        m_properties.push_back({name, [=]() -> bool {
            char buf[512];
            std::string s = value->string();
            strncpy(buf, s.c_str(), sizeof(buf));
            buf[sizeof(buf) - 1] = '\0';
            if (ImGui::InputText("##", buf, sizeof(buf))) {
                *value = buf;
                return true;
            }
            return false;
        }});
    }

    void addProperty(const std::string& name, glm::vec2* value,
                     float min = -FLT_MAX, float max = FLT_MAX, float speed = 1.f) {
        m_properties.push_back({name, [=]() -> bool {
            return ImGui::DragFloat2("##", (float*)value, speed, min, max);
        }});
        m_reflected.emplace_back(name, PinDataType::Vec2, value, min, max);
    }

    void addProperty(const std::string& name, glm::vec3* value,
                     float min = -FLT_MAX, float max = FLT_MAX, float speed = 1.f) {
        m_properties.push_back({name, [=]() -> bool {
            return ImGui::DragFloat3("##", (float*)value, speed, min, max);
        }});
        m_reflected.emplace_back(name, PinDataType::Vec3, value, min, max);
    }

    void addProperty(const std::string& name, ofColor* value) {
        m_properties.push_back({name, [value, name]() -> bool {
            ofFloatColor fc = *value;
            ImVec4 c = ImVec4(fc.r, fc.g, fc.b, fc.a);
            if (ImGui::ColorEdit4("##", (float*)&c)) {
                value->set(c.x * 255.f, c.y * 255.f, c.z * 255.f, c.w * 255.f);
                return true;
            }
            return false;
        }});
        m_reflected.emplace_back(name, PinDataType::Color, value, 0.f, 255.f);
    }

    void addProperty(const std::string& name, ofFloatColor* value) {
        m_properties.push_back({name, [value, name]() -> bool {
            ImVec4 c = ImVec4(value->r, value->g, value->b, value->a);
            if (ImGui::ColorEdit4("##", (float*)&c)) {
                value->set(c.x, c.y, c.z, c.w);
                return true;
            }
            return false;
        }});
    }

    void addProperty(const std::string& name, glm::vec4* value,
                     float min = -FLT_MAX, float max = FLT_MAX, float speed = 1.f) {
        m_properties.push_back({name, [=]() -> bool {
            return ImGui::DragFloat4("##", (float*)value, speed, min, max);
        }});
        m_reflected.emplace_back(name, PinDataType::Vec4, value, min, max);
    }

    void addProperty(const std::string& name, glm::quat* value) {
        m_properties.push_back({name, [value, name]() -> bool {
            glm::vec3 euler = glm::degrees(glm::eulerAngles(*value));
            if (ImGui::DragFloat3("##", (float*)&euler, 0.5f)) {
                *value = glm::quat(glm::radians(euler));
                return true;
            }
            return false;
        }});
        m_reflected.emplace_back(name, PinDataType::Quat, value, -1.f, 1.f);
    }

    template<typename T>
    void addEnumProperty(const std::string& name, T* value,
                         const std::vector<std::string>& options) {
        m_properties.push_back({name, [=]() -> bool {
            std::vector<const char*> labels;
            labels.reserve(options.size());
            for (const auto& s : options) labels.push_back(s.c_str());
            int cur = static_cast<int>(*value);
            if (ImGui::Combo("##", &cur, labels.data(), (int)labels.size())) {
                *value = static_cast<T>(cur);
                return true;
            }
            return false;
        }});
        m_reflected.emplace_back(name, PinDataType::Int, value, 0.f, (float)(options.size() - 1));
    }

    // UI-only custom widget (no reflection metadata)
    void addCustomProperty(const std::string& name, std::function<void()> widget) {
        m_properties.push_back({name, [widget]() -> bool {
            widget();
            return ImGui::IsItemDeactivatedAfterEdit();
        }});
    }

    // ── Data-only reflection (no UI widget) ──────────────────────────────────

    void addReflectable(const std::string& name, float* v, float min = 0.f, float max = 100.f) {
        m_reflected.emplace_back(name, PinDataType::Float, v, min, max);
    }
    void addReflectable(const std::string& name, int* v, int min = 0, int max = 100) {
        m_reflected.emplace_back(name, PinDataType::Int, v, (float)min, (float)max);
    }
    void addReflectable(const std::string& name, bool* v) {
        m_reflected.emplace_back(name, PinDataType::Bool, v, 0.f, 1.f);
    }
    void addReflectable(const std::string& name, glm::vec2* v, float min = -1000.f, float max = 1000.f) {
        m_reflected.emplace_back(name, PinDataType::Vec2, v, min, max);
    }
    void addReflectable(const std::string& name, glm::vec3* v, float min = -1000.f, float max = 1000.f) {
        m_reflected.emplace_back(name, PinDataType::Vec3, v, min, max);
    }
    void addReflectable(const std::string& name, glm::vec4* v, float min = -1000.f, float max = 1000.f) {
        m_reflected.emplace_back(name, PinDataType::Vec4, v, min, max);
    }
    void addReflectable(const std::string& name, glm::quat* v) {
        m_reflected.emplace_back(name, PinDataType::Quat, v, -1.f, 1.f);
    }
    void addReflectable(const std::string& name, ofColor* v) {
        m_reflected.emplace_back(name, PinDataType::Color, v, 0.f, 255.f);
    }

    // ── Drawing & access ─────────────────────────────────────────────────────

    bool draw() {
        bool changed = false;
        if (ImGui::CollapsingHeader(m_componentName.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
            changed = drawPanel();
        }
        return changed;
    }

    /// Draw registered widgets without a collapsing header (standalone panels).
    /// Labels sit in a left column (auto-sized to longest name); widgets use the rest.
    /// @param minLabelWidth  minimum pixels for the label column (0 = auto only).
    bool drawPanel(float minLabelWidth = 0.f) {
        bool changed = false;
        if (m_properties.empty()) {
            return false;
        }

        ImGui::PushID(m_componentName.c_str());

        float labelColW = minLabelWidth;
        for (const auto& prop : m_properties) {
            if (prop.name.empty()) {
                continue;
            }
            labelColW = std::max(labelColW, ImGui::CalcTextSize(prop.name.c_str()).x);
        }
        labelColW += ImGui::GetStyle().ItemInnerSpacing.x * 2.f;

        for (auto& prop : m_properties) {
            ImGui::PushID(prop.name.empty() ? "row" : prop.name.c_str());

            if (prop.name.empty()) {
                changed |= prop.drawImGui();
            } else {
                ImGui::AlignTextToFramePadding();
                ImGui::TextUnformatted(prop.name.c_str());
                ImGui::SameLine(labelColW);
                const float widgetW = std::max(ImGui::GetContentRegionAvail().x, 48.f);
                ImGui::SetNextItemWidth(widgetW);
                changed |= prop.drawImGui();
            }

            ImGui::PopID();
        }

        ImGui::PopID();
        return changed;
    }

    bool hasProperties() const { return !m_properties.empty(); }
    void clear()               { m_properties.clear(); m_reflected.clear(); }

    const std::vector<ReflectedProperty>& getReflectedProperties() const { return m_reflected; }

    const ReflectedProperty* findProperty(const std::string& name) const {
        for (const auto& p : m_reflected) if (p.name == name) return &p;
        return nullptr;
    }
    ReflectedProperty* findProperty(const std::string& name) {
        for (auto& p : m_reflected) if (p.name == name) return &p;
        return nullptr;
    }

    // Serialize all reflected properties to JSON via ofJSON.
    // Requires #include "PropertyValue.h" (included transitively via PropertyReflector.h)
    ofJson serialize() const;

    // Deserialize property values from JSON. Returns true if any changed.
    bool deserialize(const ofJson& j);

private:
    std::string                    m_componentName;
    std::vector<PropertyInfo>      m_properties;
    std::vector<ReflectedProperty> m_reflected;
};

// ─────────────────────────────────────────────────────────────────────────────
// Generic registerProperties hook
//
// Specialize this template for any component type that should be reflectable
// without being part of ofxEnTTKit. The specialization is then automatically
// used by the Inspector UI, ofxTanim, ofxEnTTStateCollector, and the node editor.
//
// Example:
//   template<>
//   inline void inspector::registerProperties(my_comp& c, inspector::ComponentInspector& ci) {
//       ci.addProperty("speed",  &c.speed,  0.f, 200.f);
//       ci.addProperty("active", &c.active);
//   }
// ─────────────────────────────────────────────────────────────────────────────
template<typename T>
inline void registerProperties(T& /*comp*/, ComponentInspector& /*ci*/) {
    // No-op default. Specialize for custom component types.
}

} // namespace inspector
