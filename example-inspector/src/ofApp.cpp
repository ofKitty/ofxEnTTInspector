#include "ofApp.h"

#include "systems/TransformSystem.h"
#include "transform_helpers.h"

// ─────────────────────────────────────────────────────────────────────────────

void ofApp::setup() {
    ofSetWindowTitle("ofxEnTTInspector — Entity Inspector Demo");
    ofBackground(18, 18, 22);
    ofSetFrameRate(60);

    gui.setup();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    viewport.allocate(800, 600, GL_RGBA);
    cam.setDistance(600);

    createDemoEntities();
}

// ─────────────────────────────────────────────────────────────────────────────

namespace {

void addSpatialEntity(entt::registry& reg,
                      entt::entity e,
                      const std::string& name,
                      const glm::vec3& position)
{
    reg.emplace<ecs::Relationship>(e);
    reg.emplace<ecs::LocalTransform>(e, ecs::LocalTransform{.position = position});
    reg.emplace<ecs::GlobalTransform>(e);
    reg.emplace<ecs::named_entity>(e, name);
}

} // namespace

void ofApp::createDemoEntities() {
    // Entity 1 — Box mesh
    {
        auto e = registry.create();
        addSpatialEntity(registry, e, "Box", {-150.f, 0.f, 0.f});

        auto& tag = registry.emplace<ecs::tag_component>(e);
        tag.tag = "mesh";

        auto& mesh = registry.emplace<ecs::mesh_component>(e);
        mesh.primitiveType = ecs::MESH_BOX;
        mesh.width = 100; mesh.height = 100; mesh.depth = 100;
        mesh.color = ofColor(80, 160, 240);
        mesh.rebuild();

        auto& render = registry.emplace<ecs::render_component>(e);
        render.visible = true;

        if (selected == entt::null) selected = e;
    }

    // Entity 2 — Sphere
    {
        auto e = registry.create();
        addSpatialEntity(registry, e, "Sphere", {150.f, 0.f, 0.f});

        auto& mesh = registry.emplace<ecs::mesh_component>(e);
        mesh.primitiveType = ecs::MESH_SPHERE;
        mesh.radius = 60;
        mesh.color = ofColor(240, 120, 60);
        mesh.rebuild();

        registry.emplace<ecs::render_component>(e).visible = true;
    }

    // Entity 3 — 2D Circle shape
    {
        auto e = registry.create();
        addSpatialEntity(registry, e, "Circle", {0.f, 180.f, 0.f});

        auto& circle = registry.emplace<ecs::circle_component>(e);
        circle.x = 0;
        circle.y = 0;
        circle.radius = 50;
        ecs::attachShapePaint(registry, e, ecs::ShapePaintParams{
            true, true,
            ofColor(100, 220, 140), ofColor(255), 2.f
        });

        registry.emplace<ecs::render_component>(e).visible = true;
    }

    // Entity 4 — Tagged camera stand-in
    {
        auto e = registry.create();
        addSpatialEntity(registry, e, "Camera", {0.f, -180.f, 200.f});

        auto& tag = registry.emplace<ecs::tag_component>(e);
        tag.tag = "camera";

        registry.emplace<ecs::camera_component>(e);
    }

    ecs::TransformSystem::update(registry);
}

// ─────────────────────────────────────────────────────────────────────────────

void ofApp::update() {
    for (auto [e, lt, mesh] : registry.view<ecs::LocalTransform, ecs::mesh_component>().each()) {
        (void)e;
        (void)mesh;
        const glm::quat yaw   = glm::angleAxis(glm::radians(0.4f), glm::vec3(0, 1, 0));
        const glm::quat pitch = glm::angleAxis(glm::radians(0.2f), glm::vec3(1, 0, 0));
        lt.orientation = glm::normalize(yaw * pitch * lt.orientation);
    }

    ecs::TransformSystem::update(registry);
}

// ─────────────────────────────────────────────────────────────────────────────

void ofApp::draw() {
    renderScene();

    gui.begin();

    // Full-screen dockspace
    ImGuiViewport* vp = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(vp->WorkPos);
    ImGui::SetNextWindowSize(vp->WorkSize);
    ImGui::SetNextWindowViewport(vp->ID);
    ImGuiWindowFlags hostFlags =
        ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus |
        ImGuiWindowFlags_NoNavFocus;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.07f, 0.07f, 0.09f, 1.f));
    ImGui::Begin("##Host", nullptr, hostFlags);
    ImGui::PopStyleColor();
    ImGui::PopStyleVar();
    ImGui::DockSpace(ImGui::GetID("##Dock"), ImVec2(0, 0),
                     ImGuiDockNodeFlags_PassthruCentralNode);
    ImGui::End();

    drawScenePanel();
    drawViewport();
    drawPropertiesPanel();

    gui.end();
}

// ─────────────────────────────────────────────────────────────────────────────

void ofApp::renderScene() {
    viewport.begin();
    ofClear(30, 30, 38, 255);
    cam.begin();
    ofEnableDepthTest();

    for (auto [e, mesh, render] :
         registry.view<ecs::mesh_component, ecs::render_component>().each()) {
        if (!render.visible || !ecs::hasSpatialTransform(registry, e)) continue;
        ofPushMatrix();
        ofMultMatrix(ecs::entityWorldMatrix(registry, e));
        ofSetColor(mesh.color);
        if (mesh.drawFaces)     mesh.m_mesh.draw();
        if (mesh.drawWireframe) mesh.m_mesh.drawWireframe();
        ofPopMatrix();
    }

    ofDisableDepthTest();
    cam.end();
    viewport.end();
}

// ─────────────────────────────────────────────────────────────────────────────

void ofApp::drawScenePanel() {
    ImGui::SetNextWindowSize(ImVec2(220, 400), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
    ImGui::Begin("Scene");

    ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.6f, 1.f), "Entities");
    ImGui::Separator();
    ImGui::Spacing();

    for (auto e : registry.view<ecs::named_entity>()) {
        const auto& ne = registry.get<ecs::named_entity>(e);
        std::string label = ne.getName();
        if (label.empty()) label = "Entity " + std::to_string((uint32_t)e);

        if (registry.any_of<ecs::mesh_component>(e))        label = "[ ] " + label;
        else if (registry.any_of<ecs::circle_component>(e)) label = "( ) " + label;
        else if (registry.any_of<ecs::camera_component>(e)) label = "[C] " + label;

        bool isSel = (e == selected);
        ImGui::PushID((int)e);
        if (ImGui::Selectable(label.c_str(), isSel))
            selected = e;
        ImGui::PopID();
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::TextColored(ImVec4(0.4f, 0.4f, 0.5f, 1.f),
                       "%d entities", (int)registry.view<ecs::named_entity>().size());

    ImGui::End();
}

// ─────────────────────────────────────────────────────────────────────────────

void ofApp::drawViewport() {
    ImGui::SetNextWindowSize(ImVec2(800, 500), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(220, 0), ImGuiCond_FirstUseEver);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin("Viewport");
    ImGui::PopStyleVar();

    ImVec2 avail = ImGui::GetContentRegionAvail();
    if (avail.x > 1 && avail.y > 1) {
        if ((int)avail.x != viewport.getWidth() || (int)avail.y != viewport.getHeight()) {
            viewport.allocate((int)avail.x, (int)avail.y, GL_RGBA);
        }
        ImGui::Image(
            (ImTextureID)(uintptr_t)viewport.getTexture().getTextureData().textureID,
            avail, ImVec2(0, 1), ImVec2(1, 0));
    }

    ImGui::End();
}

// ─────────────────────────────────────────────────────────────────────────────

void ofApp::drawPropertiesPanel() {
    ImGui::SetNextWindowSize(ImVec2(320, 600), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(960, 0), ImGuiCond_FirstUseEver);
    ImGui::Begin("Properties");

    if (selected == entt::null || !registry.valid(selected)) {
        ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.f),
                           "Select an entity in the Scene panel.");
        ImGui::End();
        return;
    }

    if (registry.any_of<ecs::named_entity>(selected)) {
        const std::string& name = registry.get<ecs::named_entity>(selected).getName();
        ImGui::TextColored(ImVec4(0.4f, 0.75f, 1.f, 1.f), "%s", name.c_str());
    }
    ImGui::TextColored(ImVec4(0.4f, 0.4f, 0.5f, 1.f),
                       "entity #%u", (uint32_t)selected);
    ImGui::Separator();
    ImGui::Spacing();

    inspector::inspectEntity(registry, selected);

    ImGui::End();
}
