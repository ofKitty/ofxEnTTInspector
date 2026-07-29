#pragma once

#include "imgui.h"
#include <magic_enum/magic_enum.hpp>
#include <string>

namespace inspector {

enum class PinDataType {
    Float,
    Int,
    Bool,
    String,
    Vec2,
    Vec3,
    Vec4,
    Quat,
    Color,
    Trigger,
    Entity,
    Any
};

inline bool canConnect(PinDataType a, PinDataType b) {
    if (a == PinDataType::Any || b == PinDataType::Any) return true;
    return a == b;
}

inline ImU32 getPinColor(PinDataType type) {
    switch (type) {
        case PinDataType::Float:   return IM_COL32(255, 165,   0, 255);
        case PinDataType::Int:     return IM_COL32(100, 149, 237, 255);
        case PinDataType::Bool:    return IM_COL32(220,  80,  80, 255);
        case PinDataType::String:  return IM_COL32(100, 200, 100, 255);
        case PinDataType::Vec2:    return IM_COL32(100, 200, 200, 255);
        case PinDataType::Vec3:    return IM_COL32(180, 130, 220, 255);
        case PinDataType::Vec4:    return IM_COL32(140,  90, 200, 255);
        case PinDataType::Quat:    return IM_COL32(200, 160,  60, 255);
        case PinDataType::Color:   return IM_COL32(255, 150, 200, 255);
        case PinDataType::Trigger: return IM_COL32(255, 255, 255, 255);
        case PinDataType::Entity:  return IM_COL32( 80, 180, 220, 255);
        case PinDataType::Any:     return IM_COL32(150, 150, 150, 255);
        default:                   return IM_COL32(200, 200, 200, 255);
    }
}

inline std::string getPinTypeName(PinDataType type) {
    auto name = magic_enum::enum_name(type);
    return name.empty() ? "Unknown" : std::string(name);
}

// Returns how many float curves are needed to represent this type in a timeline.
inline int getPinCurveCount(PinDataType type) {
    switch (type) {
        case PinDataType::Float:
        case PinDataType::Int:
        case PinDataType::Bool:    return 1;
        case PinDataType::Vec2:    return 2;
        case PinDataType::Vec3:    return 3;
        case PinDataType::Vec4:
        case PinDataType::Quat:
        case PinDataType::Color:   return 4;
        default:                   return 0;
    }
}

} // namespace inspector
