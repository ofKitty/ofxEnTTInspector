// ============================================================================
// Utility Component Inspectors - Implementation
// ============================================================================

#include "utility_inspectors.h"
#include "VisitFieldsInspector.h"

namespace inspector {

// ============================================================================
// Grid Helper Component Inspector
// ============================================================================

void registerProperties(ecs::grid_helper_component& comp, ComponentInspector& inspector) {
    inspector.addProperty("Size", &comp.size, 10.0f, 10000.0f);
    inspector.addProperty("Divisions", &comp.divisions, 2, 100);
    
    inspector.addProperty("Center Line Color", &comp.centerLineColor);
    inspector.addProperty("Grid Color", &comp.gridColor);
    
    inspector.addProperty("Show XY Plane", &comp.showXY);
    inspector.addProperty("Show XZ Plane", &comp.showXZ);
    inspector.addProperty("Show YZ Plane", &comp.showYZ);
    
    inspector.addProperty("Show Axes", &comp.showAxes);
    if (comp.showAxes) {
        inspector.addProperty("Axis Length", &comp.axisLength, 10.0f, 1000.0f);
    }
}

// ============================================================================
// Gizmo Component Inspector
// ============================================================================

void registerProperties(ecs::gizmo_component& comp, ComponentInspector& inspector) {
    std::vector<std::string> modes = {"Translate", "Rotate", "Scale"};
    inspector.addEnumProperty("Mode", (int*)&comp.mode, modes);
    
    inspector.addProperty("Enabled", &comp.enabled);
    inspector.addProperty("Local Space", &comp.localSpace);
    inspector.addProperty("Size", &comp.size, 10.0f, 500.0f);
    
    if (ImGui::CollapsingHeader("Colors")) {
        inspector.addProperty("X Color", &comp.xColor);
        inspector.addProperty("Y Color", &comp.yColor);
        inspector.addProperty("Z Color", &comp.zColor);
        inspector.addProperty("Selected Color", &comp.selectedColor);
    }
    
    inspector.addCustomProperty("Status", [&]() {
        if (comp.dragging) {
            ImGui::TextColored(ImVec4(1, 1, 0, 1), "Dragging");
            
            const char* axisName = "None";
            switch (comp.selectedAxis) {
                case ecs::gizmo_component::X: axisName = "X"; break;
                case ecs::gizmo_component::Y: axisName = "Y"; break;
                case ecs::gizmo_component::Z: axisName = "Z"; break;
                case ecs::gizmo_component::XY: axisName = "XY"; break;
                case ecs::gizmo_component::YZ: axisName = "YZ"; break;
                case ecs::gizmo_component::XZ: axisName = "XZ"; break;
                case ecs::gizmo_component::XYZ: axisName = "XYZ"; break;
                default: break;
            }
            ImGui::Text("Selected Axis: %s", axisName);
        }
    });
}

// ============================================================================
// Bounding Box Component Inspector
// ============================================================================

void registerProperties(ecs::bounding_box_component& comp, ComponentInspector& inspector) {
    inspector.addProperty("Min", &comp.min);
    inspector.addProperty("Max", &comp.max);
    
    inspector.addProperty("Auto Update", &comp.autoUpdate);
    inspector.addProperty("Visible", &comp.visible);
    inspector.addProperty("Color", &comp.color);
    
    inspector.addCustomProperty("Info", [&]() {
        glm::vec3 center = comp.getCenter();
        glm::vec3 size = comp.getSize();
        
        ImGui::Text("Center: (%.2f, %.2f, %.2f)", center.x, center.y, center.z);
        ImGui::Text("Size: (%.2f, %.2f, %.2f)", size.x, size.y, size.z);
        ImGui::Text("Width: %.2f", comp.getWidth());
        ImGui::Text("Height: %.2f", comp.getHeight());
        ImGui::Text("Depth: %.2f", comp.getDepth());
    });
    
    inspector.addCustomProperty("Test Point", [&]() {
        static glm::vec3 testPoint(0, 0, 0);
        ImGui::DragFloat3("Point", &testPoint.x);
        
        bool contains = comp.contains(testPoint);
        if (contains) {
            ImGui::TextColored(ImVec4(0, 1, 0, 1), "Point is INSIDE");
        } else {
            ImGui::TextColored(ImVec4(1, 0, 0, 1), "Point is OUTSIDE");
        }
    });
}

// ============================================================================
// Mask Component Inspector
// ============================================================================

void registerProperties(ecs::mask_component& comp, ComponentInspector& inspector) {
    std::vector<std::string> modes = {"Add", "Subtract", "Intersect"};
    inspector.addEnumProperty("Mode", (int*)&comp.mode, modes);
    
    inspector.addProperty("Enabled", &comp.enabled);
    inspector.addProperty("Invert Mask", &comp.invertMask);
    
    inspector.addCustomProperty("Status", [&]() {
        if (comp.isMaskAllocated) {
            ImGui::TextColored(ImVec4(0, 1, 0, 1), "Mask Allocated");
            ImGui::Text("Size: %dx%d", 
                (int)comp.maskFbo.getWidth(), 
                (int)comp.maskFbo.getHeight());
        } else {
            ImGui::TextColored(ImVec4(1, 0, 0, 1), "Mask Not Allocated");
        }
    });
    
    inspector.addCustomProperty("Actions", [&]() {
        static int width = 1920, height = 1080;
        ImGui::InputInt("Width", &width);
        ImGui::InputInt("Height", &height);
        
        if (ImGui::Button("Allocate Mask")) {
            comp.allocate(width, height);
        }
    });
}

// ============================================================================
// Rigidbody Component Inspector
// ============================================================================

void registerProperties(ecs::rigidbody_component& comp, ComponentInspector& inspector) {
    inspector.addProperty("Velocity", &comp.velocity);
    inspector.addProperty("Angular Velocity", &comp.angularVelocity);
    
    inspector.addProperty("Mass", &comp.mass, 0.1f, 1000.0f);
    inspector.addProperty("Drag", &comp.drag, 0.0f, 1.0f);
    inspector.addProperty("Angular Drag", &comp.angularDrag, 0.0f, 1.0f);
    inspector.addProperty("Restitution", &comp.restitution, 0.0f, 1.0f);
    
    inspector.addProperty("Use Gravity", &comp.useGravity);
    inspector.addProperty("Is Kinematic", &comp.isKinematic);
    
    if (ImGui::CollapsingHeader("Constraints")) {
        inspector.addProperty("Freeze Position X", &comp.freezePositionX);
        inspector.addProperty("Freeze Position Y", &comp.freezePositionY);
        inspector.addProperty("Freeze Position Z", &comp.freezePositionZ);
        inspector.addProperty("Freeze Rotation X", &comp.freezeRotationX);
        inspector.addProperty("Freeze Rotation Y", &comp.freezeRotationY);
        inspector.addProperty("Freeze Rotation Z", &comp.freezeRotationZ);
    }
    
    inspector.addCustomProperty("Apply Forces", [&]() {
        static glm::vec3 force(0, 0, 0);
        ImGui::DragFloat3("Force", &force.x, 1.0f);
        
        if (ImGui::Button("Apply Force")) {
            if (comp.mass > 0) {
                comp.velocity += force / comp.mass * 0.016f;
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Apply Impulse")) {
            if (comp.mass > 0) {
                comp.velocity += force / comp.mass;
            }
        }
        
        if (ImGui::Button("Reset Velocity")) {
            comp.velocity = glm::vec3(0, 0, 0);
            comp.angularVelocity = glm::vec3(0, 0, 0);
        }
    });
    
    inspector.addCustomProperty("Info", [&]() {
        float speed = glm::length(comp.velocity);
        float angularSpeed = glm::length(comp.angularVelocity);
        
        ImGui::Text("Speed: %.2f", speed);
        ImGui::Text("Angular Speed: %.2f", angularSpeed);
        ImGui::Text("Kinetic Energy: %.2f", 0.5f * comp.mass * speed * speed);
    });
}

// ============================================================================
// Layer Component Inspector
// ============================================================================

void registerProperties(ecs::layer_component& comp, ComponentInspector& inspector) {
    registerVisitFields(comp, inspector);
}

} // namespace inspector
