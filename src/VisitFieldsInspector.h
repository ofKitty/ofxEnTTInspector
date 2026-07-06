#pragma once

#include "ComponentInspector.h"
#include "json/component_json.h" // visitFields overloads (component_fields.h alone is insufficient)

#include <string>
#include <type_traits>
#include <vector>

namespace inspector {

/// Bind ecs::visitFields into ComponentInspector addProperty calls (shared field table).
struct InspectorFieldVisitor {
	ComponentInspector& ci;

	void operator()(const char*, const char* label, float& v, float min = 0.f, float max = 100.f)
	{
		ci.addProperty(label, &v, min, max);
	}
	void operator()(const char*, const char* label, int& v, int min = 0, int max = 100)
	{
		ci.addProperty(label, &v, min, max);
	}
	void operator()(const char*, const char* label, bool& v) { ci.addProperty(label, &v); }
	void operator()(const char*, const char* label, std::string& v) { ci.addProperty(label, &v); }
	void operator()(const char*, const char* label, glm::vec2& v, float min = -FLT_MAX, float max = FLT_MAX)
	{
		ci.addProperty(label, &v, min, max);
	}
	void operator()(const char*, const char* label, glm::vec3& v, float min = -FLT_MAX, float max = FLT_MAX)
	{
		ci.addProperty(label, &v, min, max);
	}
	void operator()(const char*, const char* label, glm::vec4& v, float min = -FLT_MAX, float max = FLT_MAX)
	{
		ci.addProperty(label, &v, min, max);
	}
	void operator()(const char*, const char* label, glm::quat& v) { ci.addProperty(label, &v); }
	void operator()(const char*, const char* label, ofColor& v) { ci.addProperty(label, &v); }
	void operator()(const char*, const char* label, of::filesystem::path& v) { ci.addProperty(label, &v); }
	void operator()(const char*, const char* label, ofRectangle& v)
	{
		ci.addCustomProperty(label, [&v]() {
			bool changed = false;
			changed |= ImGui::DragFloat("x", &v.x);
			changed |= ImGui::DragFloat("y", &v.y);
			changed |= ImGui::DragFloat("w", &v.width);
			changed |= ImGui::DragFloat("h", &v.height);
			return changed;
		});
	}
	void operator()(const char*, const char*, entt::entity&) {}
	void operator()(const char*, const char*, std::vector<glm::vec2>&) {}
	void operator()(const char*, const char*, std::vector<glm::vec3>&) {}

	template<typename E, typename = std::enable_if_t<std::is_enum_v<E>>>
	void operator()(const char*, const char* label, E& v)
	{
		ci.addCustomProperty(label, [&v]() {
			int iv = static_cast<int>(v);
			if (ImGui::DragInt("##", &iv, 1, 0, 64)) {
				v = static_cast<E>(iv);
				return true;
			}
			return false;
		});
	}
};

template<typename T>
void registerVisitFields(T& comp, ComponentInspector& ci)
{
	InspectorFieldVisitor vis{ ci };
	ecs::visitFields(comp, vis);
}

} // namespace inspector
