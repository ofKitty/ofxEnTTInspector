# ofxEnTTInspector

ImGui inspector UI for [ofxEnTTKit](https://github.com/ofKitty/ofxEnTTKit) ECS components, creating a unified reflection and serialization foundation.

---

## What it does

- Draws per-component ImGui property editors for any ECS entity.
- Exposes a shared type vocabulary (`PinDataType`) used across the ecosystem for node editors, animation, and state presets.
- Provides `PropertyValue` — an owned, serializable deep-copy of any field — enabling keyframe storage, preset snapshots, and settings persistence.

---

## Relationship to ofxKit

In the ofKitty stack, **ofxEnTTInspector owns all component field UI** — every
scalar/color/enum/path editor for a component lives here, via
`registerProperties(comp, inspector)`. **ofxKit only hosts** the Properties panel;
it calls `inspector::inspectEntity(registry, selected)` and draws the result.

Two consequences worth knowing:

- A component is only editable in Properties if it has a `registerProperties`
  overload **and** is dispatched in
  [`entity_inspector.cpp`](src/entity_inspector.cpp). Having a `registerProperties`
  overload is not enough — picker registry ≠ inspector dispatch. When you add a
  component's UI, dispatch it there too.
- Spatial manipulation (transforms, path anchors, mesh verts) is **not** a
  property and does not belong here — that is ofxKit (entity-spatial) or a domain
  *Kit such as ofxVectorKit (sub-element-spatial). See ofxKit
  `docs/addon-boundaries.md`.

---

## Dependencies

| Addon | Why |
|-------|-----|
| `ofxEnTTKit` | ECS component types |
| `ofxImGui` | ImGui rendering |
| `ofxImGuiFileDialog` | File picker widgets |

---

## Kit coverage

`ofxEnTTInspector` ships ImGui editors for **ofxEnTTKit** components and dispatches them from [`entity_inspector.cpp`](src/entity_inspector.cpp). Headers under [`src/inspectors/`](src/inspectors/):

| Inspector header | Kit components |
|------------------|----------------|
| `base_inspectors` | hierarchy, named entity, FBO, mesh/image/video, … |
| `graphics2d_inspectors` / `paint_inspectors` | 2D shapes, fill/stroke/gradient |
| `rendering_inspectors` / `animation_inspectors` | lights, materials, tween, particles |
| `hardware_inspectors` | serial, OSC, GPIO, network device, sACN, audio/MIDI sources |
| `led_inspectors` | `uv_component`, `uv_sample_component` |
| `input_inspectors` / `music_inspectors` / … | keyboard, transport, sequencers, … |

**Domain addons** keep their own inspectors and register via `inspector::addExtraEntityInspector` + `ecs::registerComponent`:

---

## Quick start


```cpp
#include "ofxEnTTInspector.h"

// Inspect all components on a selected entity:
if (inspector::inspectEntity(registry, selectedEntity)) {
    // a property was changed this frame
}

// Or inspect a single component:
inspector::ComponentInspector ci("My Component");
inspector::registerProperties(myComp, ci);
ci.draw();

// Get all reflected properties (for animation / state capture):
auto props = inspector::getEntityProperties(registry, entity);
```

---

## PinDataType

A single enum representing all animatable / connectable value types:

```cpp
enum class PinDataType {
    Float, Int, Bool, String,
    Vec2, Vec3, Vec4, Quat,   // ← Vec4 and Quat added
    Color, Trigger, Any
};
```

Helper functions:

```cpp
ImU32       inspector::getPinColor(PinDataType);     // ImGui color for node pins
const char* inspector::getPinTypeName(PinDataType);  // human-readable name
int         inspector::getPinCurveCount(PinDataType); // float curves needed for timeline
bool        inspector::canConnect(PinDataType, PinDataType);
```

---

## ReflectedProperty

`ReflectedProperty` holds a **live pointer** to a field, its name, type, and optional min/max:

```cpp
struct ReflectedProperty {
    std::string  name;
    PinDataType  type;
    void*        ptr;
    float        minVal, maxVal;

    float*      asFloat()  const;
    int*        asInt()    const;
    bool*       asBool()   const;
    std::string* asString() const;
    glm::vec2*  asVec2()   const;
    glm::vec3*  asVec3()   const;
    glm::vec4*  asVec4()   const;   // new
    glm::quat*  asQuat()   const;   // new
    ofColor*    asColor()  const;
};
```

---

## PropertyValue

`PropertyValue` is an **owned deep-copy** of a field value. Use it when you need to store, interpolate, or serialize a value independent of the component's lifetime.

```cpp
// Snapshot a live field:
auto pv = inspector::PropertyValue::from(prop);

// Write back to a live field:
pv.apply(prop);

// Lerp between two snapshots (slerp for Quat, step for Bool/String):
auto mid = inspector::PropertyValue::lerp(a, b, 0.5f);

// JSON (via ofJSON = nlohmann::json):
ofJson j   = pv.toJson();
auto   pv2 = inspector::PropertyValue::fromJson(j, PinDataType::Vec3);
```

Convenience factories: `makeFloat`, `makeInt`, `makeBool`, `makeString`, `makeVec2`, `makeVec3`, `makeVec4`, `makeQuat`, `makeColor`.

---

## PropertyReflector serialization

`PropertyReflector` (the backing store inside `ComponentInspector`) supports direct JSON round-trips:

```cpp
inspector::ComponentInspector ci("transform");
inspector::registerProperties(myComp, ci);

// Serialize all properties to JSON:
ofJson j = ci.serialize();
ofSaveJson("settings.json", j);

// Deserialize back:
ofJson j = ofLoadJson("settings.json");
ci.deserialize(j);  // writes directly through live pointers
```

---

## Registering custom components

For components outside `ofxEnTTKit`, specialize the generic template **once** — the same specialization is then shared by the Inspector UI, `ofxTanim`, and `ofxEnTTStateCollector`:

```cpp
// my_comp_inspector.h
#pragma once
#include "ComponentInspector.h"
#include "my_comp.h"

template<>
inline void inspector::registerProperties(my_comp& c, inspector::ComponentInspector& ci) {
    ci.addProperty("speed",  &c.speed,  0.f, 200.f);
    ci.addProperty("active", &c.active);
    ci.addProperty("color",  &c.color);
    ci.addProperty("offset", &c.offset, -1000.f, 1000.f);
}
```

Include `my_comp_inspector.h` wherever you use the inspector, Tanim, or StateCollector with `my_comp`.

---

## ComponentInspector API reference

```cpp
// Property registration (UI + reflection):
ci.addProperty("name",   &floatField,  min, max);
ci.addProperty("name",   &intField,    min, max);
ci.addProperty("name",   &boolField);
ci.addProperty("name",   &stringField);
ci.addProperty("name",   &vec2Field,   min, max);
ci.addProperty("name",   &vec3Field,   min, max);
ci.addProperty("name",   &vec4Field,   min, max);   // new
ci.addProperty("name",   &quatField);               // new (shows Euler DragFloat3)
ci.addProperty("name",   &ofColorField);
ci.addProperty("name",   &ofFloatColorField);

// Reflection-only (no UI):
ci.addReflectable("name", &field, ...);

// Draw UI, returns true if any value changed:
bool changed = ci.draw();

// Access the reflected properties:
const auto& props = ci.getReflectedProperties();

// Find by name:
const inspector::ReflectedProperty* prop = ci.findProperty("speed");

// Serialize / deserialize via ofJSON:
ofJson j = ci.serialize();
ci.deserialize(j);
```
