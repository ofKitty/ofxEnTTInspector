#include "interaction_inspectors.h"
#include "PropertyReflector.h"
#include "PropertyValue.h"
#include "systems/clickable_system.h"

namespace inspector {

void registerProperties(ecs::clickable_component& comp, ComponentInspector& inspector)
{
	inspector.addProperty("Enabled", &comp.enabled);
	inspector.addCustomProperty("State", [&]() {
		ImGui::TextDisabled("Hover %s  Press %s",
		                    comp.hovered ? "ON" : "off",
		                    comp.pressed ? "ON" : "off");
	});
}

void registerProperties(ecs::click_bind_component& comp, ComponentInspector& inspector)
{
	inspector.addProperty("Target Key", &comp.targetKey);
	inspector.addProperty("Value", &comp.value);
	std::vector<std::string> actions = { "Toggle Bool", "Set Float" };
	inspector.addEnumProperty("Action", &comp.action, actions);
	inspector.addCustomProperty("Target", [&]() {
		if (comp.targetEntity == entt::null)
			ImGui::TextDisabled("No target entity");
		else
			ImGui::Text("Entity: %u", static_cast<std::uint32_t>(comp.targetEntity));
	});
}

void registerProperties(ecs::flag_component& comp, ComponentInspector& inspector)
{
	inspector.addProperty("On", &comp.on);
}

void installClickBindReflectorHook()
{
	ecs::ClickableSystem::setBindApplyHook(
		[](entt::registry& reg, const ecs::click_bind_component& bind) -> bool {
			if (bind.targetEntity == entt::null || !reg.valid(bind.targetEntity))
				return false;
			if (bind.targetKey.empty())
				return false;

			auto props = getEntityProperties(reg, bind.targetEntity);
			for (auto& p : props) {
				if (p.name != bind.targetKey)
					continue;
				switch (bind.action) {
					case ecs::ClickBindAction::ToggleBool: {
						if (p.type != PinDataType::Bool || !p.asBool())
							return false;
						PropertyValue::makeBool(!*p.asBool()).apply(p);
						return true;
					}
					case ecs::ClickBindAction::SetFloat: {
						if (p.type == PinDataType::Float && p.asFloat()) {
							PropertyValue::makeFloat(bind.value).apply(p);
							return true;
						}
						if (p.type == PinDataType::Bool && p.asBool()) {
							PropertyValue::makeBool(bind.value != 0.f).apply(p);
							return true;
						}
						return false;
					}
				}
			}
			return false;
		});
}

} // namespace inspector
