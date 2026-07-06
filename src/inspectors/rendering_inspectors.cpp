// ============================================================================
// Rendering Component Inspectors - Implementation
// ============================================================================

#include "rendering_inspectors.h"

namespace inspector {

// ============================================================================
// Light Component Inspector
// ============================================================================

void registerProperties(ecs::light_component& comp, ComponentInspector& inspector) {
    std::vector<std::string> lightTypes = {"Point", "Directional", "Spot", "Area"};
    inspector.addEnumProperty("Type", (int*)&comp.type, lightTypes);
    
    inspector.addProperty("Enabled", &comp.enabled);
    inspector.addProperty("Cast Shadows", &comp.castShadows);
    inspector.addProperty("Draw Debug", &comp.drawDebug);
    
    inspector.addProperty("Ambient Color", &comp.ambientColor);
    inspector.addProperty("Diffuse Color", &comp.diffuseColor);
    inspector.addProperty("Specular Color", &comp.specularColor);
    
    if (comp.type == ecs::light_component::POINT || comp.type == ecs::light_component::SPOT) {
        inspector.addProperty("Attenuation Constant", &comp.attenuation_constant, 0.0f, 10.0f);
        inspector.addProperty("Attenuation Linear", &comp.attenuation_linear, 0.0f, 1.0f);
        inspector.addProperty("Attenuation Quadratic", &comp.attenuation_quadratic, 0.0f, 1.0f);
    }
    
    if (comp.type == ecs::light_component::SPOT) {
        inspector.addProperty("Spot Cutoff", &comp.spotCutoff, 0.0f, 90.0f);
        inspector.addProperty("Spot Concentration", &comp.spotConcentration, 0.0f, 128.0f);
    }
    
    inspector.addCustomProperty("Apply", [&]() {
        if (ImGui::Button("Apply Light Settings")) {
            comp.apply();
        }
    });
}

// ============================================================================
// Material Component Inspector
// ============================================================================

void registerProperties(ecs::material_component& comp, ComponentInspector& inspector) {
    inspector.addProperty("Ambient", &comp.ambient);
    inspector.addProperty("Diffuse", &comp.diffuse);
    inspector.addProperty("Specular", &comp.specular);
    inspector.addProperty("Emissive", &comp.emissive);
    
    inspector.addProperty("Shininess", &comp.shininess, 0.0f, 128.0f);
    
    if (ImGui::CollapsingHeader("PBR Properties")) {
        inspector.addProperty("Metallic", &comp.metallic, 0.0f, 1.0f);
        inspector.addProperty("Roughness", &comp.roughness, 0.0f, 1.0f);
        inspector.addProperty("AO", &comp.ao, 0.0f, 1.0f);
    }
    
    inspector.addProperty("Use Texture", &comp.useTexture);
    inspector.addProperty("Receive Shadows", &comp.receiveShadows);
    inspector.addProperty("Cast Shadows", &comp.castShadows);
    
    inspector.addCustomProperty("Apply", [&]() {
        if (ImGui::Button("Apply Material")) {
            comp.apply();
        }
    });
}

// ============================================================================
// Shader Component Inspector
// ============================================================================

void registerProperties(ecs::shader_component& comp, ComponentInspector& inspector) {
    inspector.addProperty("Vertex Path", &comp.vertPath);
    inspector.addProperty("Fragment Path", &comp.fragPath);
    inspector.addProperty("Geometry Path", &comp.geomPath);
    
    inspector.addProperty("Is Loaded", &comp.isLoaded);
    inspector.addProperty("Auto Reload", &comp.autoReload);
    
    inspector.addCustomProperty("Actions", [&]() {
        if (ImGui::Button("Load Shader")) {
            if (!comp.vertPath.empty() && !comp.fragPath.empty()) {
                comp.load(comp.vertPath.string(), comp.fragPath.string(), 
                         comp.geomPath.empty() ? "" : comp.geomPath.string());
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Reload")) {
            comp.reload();
        }
    });
    
    if (comp.isLoaded) {
        if (ImGui::CollapsingHeader("Uniforms")) {
            if (!comp.floatUniforms.empty()) {
                ImGui::Text("Float Uniforms:");
                for (auto& [name, value] : comp.floatUniforms) {
                    if (ImGui::DragFloat(name.c_str(), &value, 0.01f)) {
                        comp.setUniform(name, value);
                    }
                }
            }
            
            if (!comp.vec3Uniforms.empty()) {
                ImGui::Text("Vec3 Uniforms:");
                for (auto& [name, value] : comp.vec3Uniforms) {
                    if (ImGui::DragFloat3(name.c_str(), &value.x, 0.01f)) {
                        comp.setUniform(name, value);
                    }
                }
            }
            
            if (!comp.colorUniforms.empty()) {
                ImGui::Text("Color Uniforms:");
                for (auto& [name, value] : comp.colorUniforms) {
                    if (ImGui::ColorEdit4(name.c_str(), &value.r)) {
                        comp.setUniform(name, value);
                    }
                }
            }
        }
    }
}

// ============================================================================
// Texture Component Inspector
// ============================================================================

void registerProperties(ecs::texture_component& comp, ComponentInspector& inspector) {
    inspector.addCustomProperty("Type", [&]() {
        int type = (int)comp.type;
        if (ImGui::Combo("Texture Type", &type, 
            "Diffuse\0Normal\0Specular\0Roughness\0Metallic\0AO\0Emissive\0Height\0Custom\0")) {
            comp.type = (ecs::texture_component::TextureType)type;
        }
    });
    
    inspector.addProperty("Texture Unit", &comp.textureUnit, 0, 15);
    
    inspector.addCustomProperty("Status", [&]() {
        if (comp.loaded && comp.texture.isAllocated()) {
            ImGui::TextColored(ImVec4(0, 1, 0, 1), "Loaded: %dx%d", 
                (int)comp.texture.getWidth(), (int)comp.texture.getHeight());
        } else {
            ImGui::TextColored(ImVec4(1, 0.5f, 0, 1), "Not loaded");
        }
        
        if (!comp.texturePath.empty()) {
            ImGui::Text("Path: %s", comp.texturePath.filename().string().c_str());
        }
    });
    
    inspector.addCustomProperty("UV Transform", [&]() {
        ImGui::DragFloat2("Offset", &comp.offset.x, 0.01f);
        ImGui::DragFloat2("Tiling", &comp.tiling.x, 0.1f, 0.01f, 100.0f);
        ImGui::DragFloat("Rotation", &comp.rotation, 1.0f, 0.0f, 360.0f);
    });
    
    if (comp.type == ecs::texture_component::NORMAL) {
        inspector.addProperty("Normal Strength", &comp.normalStrength, 0.0f, 2.0f);
    }
    
    inspector.addCustomProperty("Actions", [&]() {
        if (ImGui::Button("Apply Settings")) {
            comp.applySettings();
        }
    });
}

// ============================================================================
// Trail Component Inspector
// ============================================================================

void registerProperties(ecs::trail_component& comp, ComponentInspector& inspector) {
    inspector.addProperty("Max Points", &comp.maxPoints, 10, 1000);
    inspector.addProperty("Lifetime", &comp.lifetime, 0.1f, 30.0f);
    
    inspector.addProperty("Color Start", &comp.colorStart);
    inspector.addProperty("Color End", &comp.colorEnd);
    
    inspector.addCustomProperty("Width", [&]() {
        ImGui::DragFloat("Width Start", &comp.widthStart, 0.1f, 0.1f, 100.0f);
        ImGui::DragFloat("Width End", &comp.widthEnd, 0.1f, 0.1f, 100.0f);
    });
    
    inspector.addProperty("Smooth", &comp.smooth);
    if (comp.smooth) {
        inspector.addProperty("Smooth Amount", &comp.smoothAmount, 1, 50);
    }
    
    inspector.addCustomProperty("Stats", [&]() {
        ImGui::Text("Active Points: %zu / %d", comp.size(), comp.maxPoints);
        float ratio = (float)comp.size() / comp.maxPoints;
        ImGui::ProgressBar(ratio, ImVec2(-1, 0));
    });
    
    inspector.addCustomProperty("Operations", [&]() {
        if (ImGui::Button("Clear Trail")) {
            comp.clear();
        }
    });
}

// ============================================================================
// Effect Component Inspectors
// ============================================================================

void registerProperties(ecs::outline_component& comp, ComponentInspector& inspector) {
    inspector.addProperty("Enabled", &comp.enabled);
    inspector.addProperty("Outline Color", &comp.outlineColor);
    inspector.addProperty("Outline Width", &comp.outlineWidth, 0.1f, 50.0f);
    inspector.addProperty("Passes", &comp.passes, 1, 10);
}

void registerProperties(ecs::shadow_component& comp, ComponentInspector& inspector) {
    inspector.addProperty("Enabled", &comp.enabled);
    
    inspector.addCustomProperty("Offset", [&]() {
        ImGui::DragFloat2("Offset", &comp.offset.x, 0.5f);
    });
    
    inspector.addProperty("Shadow Color", &comp.shadowColor);
    inspector.addProperty("Blur", &comp.blur, 0.0f, 50.0f);
    inspector.addProperty("Intensity", &comp.intensity, 0.0f, 1.0f);
}

void registerProperties(ecs::glow_component& comp, ComponentInspector& inspector) {
    inspector.addProperty("Enabled", &comp.enabled);
    inspector.addProperty("Glow Color", &comp.glowColor);
    inspector.addProperty("Intensity", &comp.intensity, 0.0f, 1.0f);
    inspector.addProperty("Radius", &comp.radius, 0.0f, 100.0f);
    inspector.addProperty("Passes", &comp.passes, 1, 10);
}

// ============================================================================
// Stub Inspectors
// ============================================================================

void registerProperties(ecs::cubemap_component& comp, ComponentInspector& inspector) {
    inspector.addCustomProperty("Info", []() {
        ImGui::TextDisabled("Cubemap inspector not implemented");
    });
}

void registerProperties(ecs::billboard_component& comp, ComponentInspector& inspector) {
    inspector.addCustomProperty("Info", []() {
        ImGui::TextDisabled("Billboard inspector not implemented");
    });
}

void registerProperties(ecs::tube_component& comp, ComponentInspector& inspector) {
    inspector.addCustomProperty("Info", []() {
        ImGui::TextDisabled("Tube inspector not implemented");
    });
}

void registerProperties(ecs::surface_component& comp, ComponentInspector& inspector) {
    inspector.addCustomProperty("Info", []() {
        ImGui::TextDisabled("Surface inspector not implemented");
    });
}

void registerProperties(ecs::instanced_mesh_component& comp, ComponentInspector& inspector) {
    inspector.addCustomProperty("Info", []() {
        ImGui::TextDisabled("Instanced Mesh inspector not implemented");
    });
}

} // namespace inspector
