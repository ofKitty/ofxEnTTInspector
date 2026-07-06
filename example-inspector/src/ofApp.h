#pragma once

#include "ofMain.h"
#include "ofxImGui.h"
#include "ofxEnTTInspector.h"
#include "systems/shape_paint_helpers.h"

class ofApp : public ofBaseApp {
public:
    void setup() override;
    void update() override;
    void draw() override;

private:
    // ── ImGui ────────────────────────────────────────────────────────────────
    ofxImGui::Gui gui;

    // ── ECS ──────────────────────────────────────────────────────────────────
    entt::registry registry;
    entt::entity   selected = entt::null;

    void createDemoEntities();

    // ── UI panels ────────────────────────────────────────────────────────────
    void drawScenePanel();
    void drawViewport();
    void drawPropertiesPanel();

    // ── Viewport render ──────────────────────────────────────────────────────
    ofFbo viewport;
    ofEasyCam cam;
    void renderScene();
};
