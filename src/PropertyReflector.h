#pragma once

#include "PinDataTypes.h"
#include "ofColor.h"
#include "ofJson.h"
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <entt/entity/fwd.hpp>

namespace inspector {

struct ReflectedProperty {
    std::string  name;
    PinDataType  type;
    void*        ptr;
    float        minVal = 0.0f;
    float        maxVal = 100.0f;

    ReflectedProperty() = default;
    ReflectedProperty(const std::string& n, PinDataType t, void* p,
                      float minV = 0.0f, float maxV = 100.0f)
        : name(n), type(t), ptr(p), minVal(minV), maxVal(maxV) {}

    float*       asFloat()  const { return type == PinDataType::Float  ? static_cast<float*>(ptr)       : nullptr; }
    int*         asInt()    const { return type == PinDataType::Int    ? static_cast<int*>(ptr)         : nullptr; }
    bool*        asBool()   const { return type == PinDataType::Bool   ? static_cast<bool*>(ptr)        : nullptr; }
    std::string* asString() const { return type == PinDataType::String ? static_cast<std::string*>(ptr) : nullptr; }
    glm::vec2*   asVec2()   const { return type == PinDataType::Vec2   ? static_cast<glm::vec2*>(ptr)   : nullptr; }
    glm::vec3*   asVec3()   const { return type == PinDataType::Vec3   ? static_cast<glm::vec3*>(ptr)   : nullptr; }
    glm::vec4*   asVec4()   const { return type == PinDataType::Vec4   ? static_cast<glm::vec4*>(ptr)   : nullptr; }
    glm::quat*   asQuat()   const { return type == PinDataType::Quat   ? static_cast<glm::quat*>(ptr)   : nullptr; }
    ofColor*     asColor()  const { return type == PinDataType::Color  ? static_cast<ofColor*>(ptr)     : nullptr; }
};

class PropertyReflector {
public:
    void add(const std::string& n, float* v, float min = 0.f, float max = 100.f)         { m_props.emplace_back(n, PinDataType::Float,  v, min, max); }
    void add(const std::string& n, int*   v, int   min = 0,   int   max = 100)           { m_props.emplace_back(n, PinDataType::Int,    v, (float)min, (float)max); }
    void add(const std::string& n, bool*  v)                                              { m_props.emplace_back(n, PinDataType::Bool,   v, 0.f, 1.f); }
    void add(const std::string& n, std::string* v)                                        { m_props.emplace_back(n, PinDataType::String, v, 0.f, 0.f); }
    void add(const std::string& n, glm::vec2* v, float min = -1000.f, float max = 1000.f){ m_props.emplace_back(n, PinDataType::Vec2,   v, min, max); }
    void add(const std::string& n, glm::vec3* v, float min = -1000.f, float max = 1000.f){ m_props.emplace_back(n, PinDataType::Vec3,   v, min, max); }
    void add(const std::string& n, glm::vec4* v, float min = -1000.f, float max = 1000.f){ m_props.emplace_back(n, PinDataType::Vec4,   v, min, max); }
    void add(const std::string& n, glm::quat* v)                                          { m_props.emplace_back(n, PinDataType::Quat,   v, -1.f, 1.f); }
    void add(const std::string& n, ofColor* v)                                            { m_props.emplace_back(n, PinDataType::Color,  v, 0.f, 255.f); }

    const std::vector<ReflectedProperty>& properties() const { return m_props; }
    std::vector<ReflectedProperty>&       properties()       { return m_props; }

    const ReflectedProperty* find(const std::string& name) const {
        for (const auto& p : m_props) if (p.name == name) return &p;
        return nullptr;
    }
    ReflectedProperty* find(const std::string& name) {
        for (auto& p : m_props) if (p.name == name) return &p;
        return nullptr;
    }

    bool   empty() const { return m_props.empty(); }
    size_t size()  const { return m_props.size(); }
    void   clear()       { m_props.clear(); }

    // Serialize all properties to JSON using ofJSON (nlohmann).
    // Non-serializable types (Trigger, Any) are skipped.
    ofJson serialize() const;

    // Deserialize property values from JSON.
    // Only updates properties whose names are present in the JSON object.
    // Returns true if at least one property was updated.
    bool deserialize(const ofJson& j);

private:
    std::vector<ReflectedProperty> m_props;
};

// Returns all reflected properties for an entity.
std::vector<ReflectedProperty> getEntityProperties(entt::registry& reg, entt::entity e);

} // namespace inspector
