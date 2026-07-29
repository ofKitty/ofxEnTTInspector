// ============================================================================
// Base Component Inspectors - Implementation
// pfd replaced with ImGuiFileDialog (IGFD) for cross-platform support.
// ============================================================================

#include "base_inspectors.h"
#include "VisitFieldsInspector.h"
#include "ofxImGuiFileDialog.h"
#include "ofMain.h"
#include <cstdio>
#include <map>
#include <fstream>

namespace inspector {

namespace {

void drawReadOnlyParentId(entt::registry& registry, entt::entity entity)
{
    if (registry.any_of<ecs::Relationship>(entity)) {
        entt::entity p = registry.get<ecs::Relationship>(entity).parent;
        if (p != entt::null && registry.valid(p) && registry.any_of<ecs::named_entity>(p))
            ImGui::TextUnformatted(std::to_string(registry.get<ecs::named_entity>(p).getId()).c_str());
        else if (p != entt::null && registry.valid(p))
            ImGui::TextUnformatted(std::to_string(static_cast<std::uint32_t>(p)).c_str());
        else
            ImGui::TextDisabled("—");
    } else {
        ImGui::TextDisabled("—");
    }
}

} // namespace

// ============================================================================
// LocalTransform Inspector
// ============================================================================

void registerProperties(ecs::LocalTransform& comp, ComponentInspector& inspector) {
    inspector.addCustomProperty("Position", [&comp]() {
        ImGui::DragFloat3("##LocalPosition", &comp.position.x, 1.0f);
    }, 3);

    inspector.addCustomProperty("Rotation", [&comp]() {
        static std::map<void*, glm::vec3> rotStore;
        glm::vec3& rot = rotStore[&comp];
        if (!ImGui::IsAnyItemActive())
            rot = glm::degrees(glm::eulerAngles(comp.orientation));

        if (ImGui::DragFloat3("##LocalRotation", &rot.x, 0.5f))
            comp.orientation = glm::quat(glm::radians(rot));

        if (ImGui::Button("Reset Rotation##LocalTransform")) {
            comp.orientation = glm::quat(1, 0, 0, 0);
            rot = glm::vec3(0.f);
        }
    }, 3);

    inspector.addCustomProperty("Scale", [&comp]() {
        ImGui::DragFloat3("##LocalScale", &comp.scale.x, 0.01f, 0.01f, 10.f);
    }, 3);
}

// ============================================================================
// skew_component Inspector (degrees; applied as T·R·Shear·S)
// ============================================================================

void registerProperties(ecs::skew_component& comp, ComponentInspector& inspector) {
    registerVisitFields(comp, inspector);
}

// ============================================================================
// Relationship Inspector (read-only parent)
// ============================================================================

void registerProperties(ecs::Relationship& comp, ComponentInspector& inspector,
                        entt::registry& registry, entt::entity entity) {
    (void)comp;
    inspector.addCustomProperty("Parent", [&registry, entity]() {
        drawReadOnlyParentId(registry, entity);
    });
    inspector.addCustomProperty("Children", [&registry, entity]() {
        if (registry.any_of<ecs::Relationship>(entity))
            ImGui::Text("%zu", registry.get<ecs::Relationship>(entity).children_count);
        else
            ImGui::TextDisabled("—");
    });
}

// ============================================================================
// named_entity Inspector
// ============================================================================

void registerProperties(ecs::named_entity& comp, ComponentInspector& inspector) {
    inspector.addProperty("Name", &comp.name);
}

void registerProperties(ecs::named_entity& comp, ComponentInspector& inspector,
                        entt::registry& registry, entt::entity entity) {
    inspector.addCustomProperty("Entity ID", [&comp]() {
        ImGui::TextUnformatted(std::to_string(comp.getId()).c_str());
    });
    inspector.addCustomProperty("Parent ID", [&registry, entity]() {
        drawReadOnlyParentId(registry, entity);
    });
    inspector.addCustomProperty("Entity (runtime)", [entity]() {
        ImGui::TextUnformatted(std::to_string(static_cast<std::uint32_t>(entity)).c_str());
    });

    registerProperties(comp, inspector);

    inspector.addCustomProperty("Origin", [&registry, entity]() {
        if (!registry.all_of<ecs::LocalTransform>(entity))
            return;

        inspector::TransformOrigin cur = inspector::TransformOrigin::Center;
        if (registry.any_of<inspector::transform_origin_component>(entity))
            cur = registry.get<inspector::transform_origin_component>(entity).origin;

        const char* items[] = { "Center", "Top Left", "Top Right", "Bottom Left", "Bottom Right" };
        int idx = static_cast<int>(cur);
        if (ImGui::Combo("##Origin", &idx, items, 5)) {
            inspector::TransformOrigin next = static_cast<inspector::TransformOrigin>(idx);
            if (next != cur) {
                float w = 0, h = 0;
                if (registry.any_of<ecs::image_component>(entity)) {
                    auto& ic = registry.get<ecs::image_component>(entity);
                    if (ic.image.isAllocated()) { w = (float)ic.image.getWidth(); h = (float)ic.image.getHeight(); }
                } else if (registry.any_of<ecs::fbo_component>(entity)) {
                    auto& fc = registry.get<ecs::fbo_component>(entity);
                    if (fc.fbo.isAllocated()) { w = fc.fbo.getWidth(); h = fc.fbo.getHeight(); }
                }
                glm::vec2 oldOff = inspector::getOriginOffset(cur,  w, h);
                glm::vec2 newOff = inspector::getOriginOffset(next, w, h);
                auto& lt = registry.get<ecs::LocalTransform>(entity);
                lt.position.x += oldOff.x - newOff.x;
                lt.position.y += oldOff.y - newOff.y;
                if (!registry.any_of<inspector::transform_origin_component>(entity))
                    registry.emplace<inspector::transform_origin_component>(entity);
                registry.get<inspector::transform_origin_component>(entity).origin = next;
            }
        }
    });
}

// ============================================================================
// FBO Component Inspector
// ============================================================================

// Helper: shared FBO export widget using IGFD (called every frame by addCustomProperty)
static void drawFboExportWidget(ecs::fbo_component& comp) {
    if (!comp.fbo.isAllocated()) { ImGui::TextDisabled("FBO not allocated"); return; }

    // Static capture buffer — only one FBO export dialog open at a time
    static ofPixels s_pixels;
    static int s_w = 0, s_h = 0;

    if (ImGui::Button("PNG...")) {
        comp.fbo.readToPixels(s_pixels);
        s_w = s_pixels.getWidth(); s_h = s_pixels.getHeight();
        IGFD::FileDialogConfig cfg; cfg.path = ofToDataPath("", true); cfg.fileName = "canvas.png";
        ImGuiFileDialog::Instance()->OpenDialog("FboSavePng", "Save Canvas as PNG", ".png", cfg);
    }
    ImGui::SameLine();
    if (ImGui::Button("JPG...")) {
        comp.fbo.readToPixels(s_pixels);
        s_w = s_pixels.getWidth(); s_h = s_pixels.getHeight();
        IGFD::FileDialogConfig cfg; cfg.path = ofToDataPath("", true); cfg.fileName = "canvas.jpg";
        ImGuiFileDialog::Instance()->OpenDialog("FboSaveJpg", "Save Canvas as JPG", ".jpg", cfg);
    }
    ImGui::SameLine();
    if (ImGui::Button("SVG...")) {
        comp.fbo.readToPixels(s_pixels);
        s_w = s_pixels.getWidth(); s_h = s_pixels.getHeight();
        IGFD::FileDialogConfig cfg; cfg.path = ofToDataPath("", true); cfg.fileName = "canvas.svg";
        ImGuiFileDialog::Instance()->OpenDialog("FboSaveSvg", "Save Canvas as SVG", ".svg", cfg);
    }

    // Display dialogs every frame
    ImVec2 minSz(500, 350), maxSz(900, 700);

    if (ImGuiFileDialog::Instance()->Display("FboSavePng", ImGuiWindowFlags_NoCollapse, minSz, maxSz)) {
        if (ImGuiFileDialog::Instance()->IsOk())
            ofSaveImage(s_pixels, ImGuiFileDialog::Instance()->GetFilePathName());
        ImGuiFileDialog::Instance()->Close();
    }
    if (ImGuiFileDialog::Instance()->Display("FboSaveJpg", ImGuiWindowFlags_NoCollapse, minSz, maxSz)) {
        if (ImGuiFileDialog::Instance()->IsOk())
            ofSaveImage(s_pixels, ImGuiFileDialog::Instance()->GetFilePathName());
        ImGuiFileDialog::Instance()->Close();
    }
    if (ImGuiFileDialog::Instance()->Display("FboSaveSvg", ImGuiWindowFlags_NoCollapse, minSz, maxSz)) {
        if (ImGuiFileDialog::Instance()->IsOk()) {
            std::string dest = ImGuiFileDialog::Instance()->GetFilePathName();
            std::string pngPath = dest;
            size_t dot = pngPath.rfind('.');
            if (dot != std::string::npos) pngPath = pngPath.substr(0, dot) + ".png";
            if (ofSaveImage(s_pixels, pngPath)) {
                std::ofstream svg(dest);
                if (svg) {
                    std::string base = pngPath;
                    size_t slash = base.find_last_of("/\\");
                    if (slash != std::string::npos) base = base.substr(slash + 1);
                    svg << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                        << "<svg xmlns=\"http://www.w3.org/2000/svg\" "
                        << "xmlns:xlink=\"http://www.w3.org/1999/xlink\" "
                        << "width=\"" << s_w << "\" height=\"" << s_h << "\">\n"
                        << "<image width=\"" << s_w << "\" height=\"" << s_h << "\" "
                        << "xlink:href=\"" << base << "\"/>\n</svg>\n";
                }
            }
        }
        ImGuiFileDialog::Instance()->Close();
    }

    ImGui::TextDisabled("Canvas export is raster. SVG wraps a PNG.");
}

void registerProperties(ecs::fbo_component& comp, ComponentInspector& inspector) {
    inspector.addCustomProperty("Clear Frame", [&comp]() {
        bool cf = comp.clearFrame;
        if (ImGui::Checkbox("##ClearFrame", &cf)) { comp.clearFrame = cf; if (cf) comp.dirty = true; }
    });
    inspector.addCustomProperty("Clear Color", [&comp]() {
        float c[4] = { comp.clearColor.r/255.f, comp.clearColor.g/255.f, comp.clearColor.b/255.f, comp.clearColor.a/255.f };
        if (ImGui::ColorEdit4("##ClearColor", c))
            comp.setClearColor(ofColor(c[0]*255, c[1]*255, c[2]*255, c[3]*255));
    });
    inspector.addCustomProperty("Size", [&comp]() {
        int w = comp.width, h = comp.height;
        ImGui::PushItemWidth(80);
        bool changed = ImGui::InputInt("W##FboW", &w, 0, 0);
        ImGui::SameLine();
        changed |= ImGui::InputInt("H##FboH", &h, 0, 0);
        ImGui::PopItemWidth();
        if (changed) { w = std::max(1, std::min(w, 8192)); h = std::max(1, std::min(h, 8192)); comp.width = w; comp.height = h; }
        if (comp.fbo.isAllocated() && ((int)comp.fbo.getWidth() != w || (int)comp.fbo.getHeight() != h)) {
            ImGui::SameLine();
            if (ImGui::Button("Apply##FboSize")) comp.reallocate();
        }
    });
    inspector.addCustomProperty("Internal Format", [&comp]() {
        static const char* fmtNames[] = { "GL_RGB","GL_RGBA","GL_RGBA8","GL_RGBA16F","GL_RGBA32F","GL_RGB16F","GL_RGB32F" };
        static const int   fmtVals[]  = { GL_RGB,  GL_RGBA,  GL_RGBA8,  GL_RGBA16F,  GL_RGBA32F,  GL_RGB16F,  GL_RGB32F  };
        static const int   nFmt = 7;
        int idx = 1;
        for (int i = 0; i < nFmt; i++) if (fmtVals[i] == comp.internalFormat) { idx = i; break; }
        ImGui::PushItemWidth(120);
        if (ImGui::Combo("##FboFmt", &idx, fmtNames, nFmt)) comp.internalFormat = fmtVals[idx];
        ImGui::PopItemWidth();
        if (comp.fbo.isAllocated()) { ImGui::SameLine(); if (ImGui::Button("Apply##FboFmt")) comp.reallocate(); }
    });
    inspector.addCustomProperty("Texture Filter", [&comp]() {
        if (!comp.fbo.isAllocated()) return;
        int item = (comp.fbo.getTexture().getTextureData().magFilter == GL_NEAREST) ? 0 : 1;
        ImGui::PushItemWidth(120);
        if (ImGui::Combo("##TexFilter", &item, "Nearest (Pixel)\0Linear (Smooth)\0")) {
            GLint f = item == 0 ? GL_NEAREST : GL_LINEAR;
            comp.fbo.getTexture().setTextureMinMagFilter(f, f);
        }
        ImGui::PopItemWidth();
    });
    inspector.addCustomProperty("Export", [&comp]() { drawFboExportWidget(comp); });
}

void registerProperties(ecs::fbo_component& comp, ComponentInspector& inspector,
                        entt::registry& /*registry*/, entt::entity /*entity*/) {
    // Same as basic version — no app-specific actions in ofxEnTTInspector
    registerProperties(comp, inspector);
}

// ============================================================================
// FBO Reference Component Inspector (Canvas Instance)
// ============================================================================

void registerProperties(ecs::fbo_reference_component& comp, ComponentInspector& inspector,
                        entt::registry& registry, entt::entity /*entity*/) {
    inspector.addCustomProperty("Source Canvas", [&comp, &registry]() {
        auto canvases = registry.view<ecs::fbo_component, ecs::named_entity>();
        const char* cur = "None";
        if (comp.sourceEntity != entt::null && registry.valid(comp.sourceEntity))
            if (registry.any_of<ecs::named_entity>(comp.sourceEntity))
                cur = registry.get<ecs::named_entity>(comp.sourceEntity).getName().c_str();

        ImGui::SetNextItemWidth(-1);
        if (ImGui::BeginCombo("##SourceCanvas", cur)) {
            if (ImGui::Selectable("None", comp.sourceEntity == entt::null)) comp.sourceEntity = entt::null;
            ImGui::Separator();
            for (auto e : canvases) {
                auto& nc = canvases.get<ecs::named_entity>(e);
                bool sel = (e == comp.sourceEntity);
                if (ImGui::Selectable(nc.getName().c_str(), sel)) comp.sourceEntity = e;
                if (sel) ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
    });
    inspector.addCustomProperty("Source Info", [&comp, &registry]() {
        if (comp.isValid(registry)) {
            auto& fbo = registry.get<ecs::fbo_component>(comp.sourceEntity);
            ImGui::Text("Size: %d x %d", fbo.width, fbo.height);
        } else { ImGui::TextDisabled("No valid source"); }
    });
    inspector.addProperty("Show Border", &comp.showBorder);
}

// ============================================================================
// Camera Component Inspector
// ============================================================================

void registerProperties(ecs::camera_component& comp, ComponentInspector& inspector) {
    inspector.addCustomProperty("Orthographic", [&comp]() {
        bool ortho = comp.camera.getOrtho();
        if (ImGui::Checkbox("Orthographic", &ortho)) {
            if (ortho) comp.camera.enableOrtho();
            else { if (comp.camera.getNearClip() <= 0) comp.camera.setNearClip(10.f); comp.camera.disableOrtho(); }
        }
    });
    inspector.addCustomProperty("V Flip", [&comp]() {
        bool vf = comp.camera.isVFlipped();
        if (ImGui::Checkbox("V Flip", &vf)) comp.camera.setVFlip(vf);
    });
    inspector.addCustomProperty("FOV", [&comp]() {
        float fov = comp.camera.getFov();
        if (ImGui::DragFloat("FOV", &fov, 0.1f, 0.1f, 179.9f)) comp.camera.setFov(fov);
    });
    inspector.addCustomProperty("Near Clip", [&comp]() {
        float v = comp.camera.getNearClip();
        float mn = comp.camera.getOrtho() ? -10000.f : 0.001f;
        if (ImGui::DragFloat("Near Clip", &v, 1.f, mn, 10000.f)) comp.camera.setNearClip(v);
    });
    inspector.addCustomProperty("Far Clip", [&comp]() {
        float v = comp.camera.getFarClip();
        if (ImGui::DragFloat("Far Clip", &v, 10.f, 1.f, 100000.f)) comp.camera.setFarClip(v);
    });
    inspector.addProperty("Draw Frustum", &comp.drawFrustum);
}

// ============================================================================
// Render Component Inspector
// ============================================================================

void registerProperties(ecs::render_component& comp, ComponentInspector& inspector) {
    registerVisitFields(comp, inspector);
}

// ============================================================================
// Selectable Component Inspector
// ============================================================================

void registerProperties(ecs::selectable_component& comp, ComponentInspector& inspector) {
    inspector.addProperty("Selected", &comp.selected);
}

// ============================================================================
// Tag Component Inspector
// ============================================================================

void registerProperties(ecs::tag_component& comp, ComponentInspector& inspector) {
    inspector.addCustomProperty("Tags", [&comp]() {
        for (size_t i = 0; i < comp.tags.size(); ++i) {
            ImGui::PushID(static_cast<int>(i));
            char buf[256];
            std::snprintf(buf, sizeof(buf), "%s", comp.tags[i].c_str());
            if (ImGui::InputText("##tag", buf, sizeof(buf)))
                comp.tags[i] = buf;
            ImGui::SameLine();
            if (ImGui::SmallButton("-")) {
                comp.tags.erase(comp.tags.begin() + static_cast<std::ptrdiff_t>(i));
                ImGui::PopID();
                break;
            }
            ImGui::PopID();
        }
        if (ImGui::SmallButton("+ Add tag"))
            comp.tags.emplace_back();
    });
}

// ============================================================================
// Mesh Component Inspector
// ============================================================================

void registerProperties(ecs::mesh_component& comp, ComponentInspector& inspector) {
    std::vector<std::string> primTypes = {"Custom","Box","Sphere","Cone","Cylinder","Plane","Icosphere","Icosahedron","Skybox"};
    inspector.addCustomProperty("Type", [&comp, primTypes]() {
        std::vector<const char*> labels;
        labels.reserve(primTypes.size());
        for (const auto& s : primTypes) labels.push_back(s.c_str());
        int cur = (int)comp.primitiveType;
        if (ImGui::Combo("##primType", &cur, labels.data(), (int)labels.size())) {
            comp.primitiveType = (ecs::eMeshPrimitiveType)cur;
            comp.rebuild();
        }
    });
    // Float properties for Machine Get/Set Property (and modulators).
    inspector.addProperty("Radius", &comp.radius);
    inspector.addProperty("Width",  &comp.width);
    inspector.addProperty("Height", &comp.height);
    inspector.addProperty("Depth",  &comp.depth);
    inspector.addCustomProperty("Dimensions", [&comp]() {
        bool changed = false;
        if (comp.primitiveType == ecs::MESH_BOX || comp.primitiveType == ecs::MESH_PLANE) {
            changed |= ImGui::DragFloat("Width##dim",  &comp.width,  1.f, 0.01f, 1000.f);
            changed |= ImGui::DragFloat("Height##dim", &comp.height, 1.f, 0.01f, 1000.f);
            if (comp.primitiveType == ecs::MESH_BOX)
                changed |= ImGui::DragFloat("Depth##dim", &comp.depth, 1.f, 0.01f, 1000.f);
        }
        if (comp.primitiveType == ecs::MESH_SPHERE || comp.primitiveType == ecs::MESH_CONE ||
            comp.primitiveType == ecs::MESH_CYLINDER || comp.primitiveType == ecs::MESH_ICOSPHERE ||
            comp.primitiveType == ecs::MESH_ICOSAHEDRON)
            changed |= ImGui::DragFloat("Radius##dim", &comp.radius, 0.01f, 0.01f, 500.f);
        if (comp.primitiveType == ecs::MESH_CONE || comp.primitiveType == ecs::MESH_CYLINDER)
            changed |= ImGui::DragFloat("Height##dim", &comp.height, 1.f, 0.01f, 1000.f);
        if (comp.primitiveType != ecs::MESH_CUSTOM && comp.primitiveType != ecs::MESH_ICOSAHEDRON)
            changed |= ImGui::DragInt("Resolution", &comp.resolution, 0.1f, 1, 10);
        if (changed && comp.primitiveType != ecs::MESH_CUSTOM) comp.rebuild();
    });
    inspector.addProperty("Color",          &comp.color);
    inspector.addProperty("Draw Faces",     &comp.drawFaces);
    inspector.addProperty("Draw Wireframe", &comp.drawWireframe);
    inspector.addCustomProperty("Info", [&comp]() {
        ImGui::Text("Vertices: %zu  Indices: %zu",
            comp.m_mesh.getNumVertices(), comp.m_mesh.getNumIndices());
    });
}

// ============================================================================
// Image Component Inspector
// ============================================================================

void registerProperties(ecs::image_component& comp, ComponentInspector& inspector) {
    inspector.addCustomProperty("Info", [&comp]() {
        if (comp.image.isAllocated())
            ImGui::Text("Size: %dx%d", (int)comp.image.getWidth(), (int)comp.image.getHeight());
        else ImGui::TextDisabled("Not allocated");
    });
}

void registerProperties(ecs::image_component& comp, ComponentInspector& inspector,
                        entt::registry& registry, entt::entity entity) {
    inspector.addCustomProperty("File", [&registry, entity]() {
        if (auto* pc = registry.try_get<ecs::filepath_component>(entity))
            ImGui::TextUnformatted(pc->getFileName().c_str());
        else ImGui::TextDisabled("No path");
    });
    inspector.addCustomProperty("", [&comp, &registry, entity]() {
        if (!comp.image.isAllocated()) { ImGui::TextDisabled("Image not allocated"); return; }
        const float maxW = ImGui::GetContentRegionAvail().x;
        float scale = std::min(maxW / (float)comp.image.getWidth(), 1.f);
        glm::vec2 sz = glm::vec2(comp.image.getWidth(), comp.image.getHeight()) * scale;
        ImGui::Image((ImTextureID)(uintptr_t)comp.image.getTexture().getTextureData().textureID, ImVec2(sz.x, sz.y));

        // Pixel dimensions
        const int pw = (int)comp.image.getWidth(), ph = (int)comp.image.getHeight();
        // File size from filepath_component (if present)
        std::string sizeStr;
        if (auto* pc = registry.try_get<ecs::filepath_component>(entity)) {
            std::error_code ec;
            auto bytes = of::filesystem::file_size(pc->path, ec);
            if (!ec && bytes > 0) {
                if (bytes >= 1024 * 1024)
                    sizeStr = ofToString((float)bytes / (1024.f * 1024.f), 1) + " MB";
                else
                    sizeStr = ofToString((int)(bytes / 1024)) + " KB";
            }
        }
        if (sizeStr.empty())
            ImGui::Text("%dx%d px", pw, ph);
        else
            ImGui::Text("%dx%d px  •  %s", pw, ph, sizeStr.c_str());
    });
}

// ============================================================================
// Video Component Inspector
// ============================================================================

void registerProperties(ecs::video_component& comp, ComponentInspector& inspector) {
    inspector.addCustomProperty("Info", [&comp]() {
        if (comp.videoPlayer.isLoaded())
            ImGui::Text("Size: %.0fx%.0f", comp.videoPlayer.getWidth(), comp.videoPlayer.getHeight());
        else ImGui::TextDisabled("Not loaded");
    });
}

void registerProperties(ecs::video_component& comp, ComponentInspector& inspector,
                        entt::registry& registry, entt::entity entity) {
    inspector.addCustomProperty("File", [&registry, entity]() {
        if (auto* pc = registry.try_get<ecs::filepath_component>(entity))
            ImGui::TextUnformatted(pc->getFileName().c_str());
        else ImGui::TextDisabled("No path");
    });
    inspector.addCustomProperty("", [&comp, &registry, entity]() {
        if (!comp.videoPlayer.isLoaded()) return;
        const float maxW = ImGui::GetContentRegionAvail().x;
        float scale = std::min({ maxW / comp.videoPlayer.getWidth(), 1.f });
        glm::vec2 sz(comp.videoPlayer.getWidth() * scale, comp.videoPlayer.getHeight() * scale);
        ImGui::Image((ImTextureID)(uintptr_t)comp.videoPlayer.getTexture().getTextureData().textureID, ImVec2(sz.x, sz.y));

        // Pixel dimensions + file size
        const int pw = (int)comp.videoPlayer.getWidth(), ph = (int)comp.videoPlayer.getHeight();
        std::string sizeStr;
        if (auto* pc = registry.try_get<ecs::filepath_component>(entity)) {
            std::error_code ec;
            auto bytes = of::filesystem::file_size(pc->path, ec);
            if (!ec && bytes > 0) {
                if (bytes >= 1024 * 1024)
                    sizeStr = ofToString((float)bytes / (1024.f * 1024.f), 1) + " MB";
                else
                    sizeStr = ofToString((int)(bytes / 1024)) + " KB";
            }
        }
        if (sizeStr.empty())
            ImGui::Text("%dx%d px", pw, ph);
        else
            ImGui::Text("%dx%d px  •  %s", pw, ph, sizeStr.c_str());

        int frame = comp.videoPlayer.getCurrentFrame();
        ImGui::SetNextItemWidth(sz.x);
        if (ImGui::SliderInt("Frame", &frame, 0, comp.videoPlayer.getTotalNumFrames()))
            comp.videoPlayer.setFrame(frame);
        if (comp.videoPlayer.isPaused() || !comp.videoPlayer.isPlaying()) { if (ImGui::Button("Play")) comp.videoPlayer.play(); }
        else { if (ImGui::Button("Pause")) comp.videoPlayer.setPaused(true); }
        ImGui::SameLine(); ImGui::Text("Duration: %.2fs", comp.videoPlayer.getDuration());
    });
}

// ============================================================================
// Model Component Inspector
// ============================================================================

void registerProperties(ecs::model_component& comp, ComponentInspector& inspector) {
    inspector.addCustomProperty("Info", [&comp]() {
        if (comp.model.hasMeshes()) ImGui::Text("Meshes: %u", (unsigned)comp.model.getMeshCount());
        else ImGui::TextDisabled("No model loaded");
    });
}

void registerProperties(ecs::model_component& comp, ComponentInspector& inspector,
                        entt::registry& registry, entt::entity entity) {
    inspector.addCustomProperty("File", [&registry, entity]() {
        if (auto* pc = registry.try_get<ecs::filepath_component>(entity))
            ImGui::TextUnformatted(pc->getFileName().c_str());
        else ImGui::TextDisabled("No path");
    });
    registerProperties(comp, inspector);
}

// ============================================================================
// Resource Component Inspector
// ============================================================================

void registerProperties(ecs::resource_component& comp, ComponentInspector& inspector) {
    inspector.addCustomProperty("Type", [&comp]() {
        const char* names[] = {"Unknown","Image","Video","Audio","Model","Cubemap"};
        int idx = (int)comp.getType();
        ImGui::Text("Type: %s", (idx >= 0 && idx < 6) ? names[idx] : "Unknown");
    });
    inspector.addCustomProperty("Status", [&comp]() {
        const char* s[] = {"","OK","Missing"};
        int idx = (int)comp.status;
        if (idx >= 1 && idx < (int)ecs::ERS_COUNT) ImGui::Text("Status: %s", s[idx]);
    });
}

void registerProperties(ecs::resource_component& comp, ComponentInspector& inspector,
                        entt::registry& registry, entt::entity entity) {
    registerProperties(comp, inspector);
    inspector.addCustomProperty("File", [&registry, entity]() {
        if (auto* pc = registry.try_get<ecs::filepath_component>(entity)) {
            char buf[512];
            std::string path = pc->getString();
            strncpy(buf, path.c_str(), sizeof(buf) - 1); buf[sizeof(buf)-1] = '\0';
            ImGui::SetNextItemWidth(-1);
            if (ImGui::InputText("##Path", buf, sizeof(buf)))
                registry.patch<ecs::filepath_component>(entity, [buf](auto& c){ c.path = std::string(buf); });
        } else { ImGui::TextDisabled("No path component"); }
    });
}

} // namespace inspector
