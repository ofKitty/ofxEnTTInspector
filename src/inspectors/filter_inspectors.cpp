#include "filter_inspectors.h"
#include "components/generator_components.h"
#include "imgui.h"
#include "components/base_components.h"

namespace inspector {

static const char* s_ditherTypeNames[] = {
    "Floyd-Steinberg", "Ordered 2x2", "Ordered 4x4", "Ordered 8x8",
    "Bayer 2x2", "Bayer 4x4", "Bayer 8x8", "Random"
};

static const char* s_pixelStyleNames[] = {
    "Rectangle", "Circle", "Rounded Rect", "Diamond", "CMYK", "RGB", "Halftone"
};

static const char* s_easingTypeNames[] = {
    "Linear",
    "Ease In Quad", "Ease Out Quad", "Ease In/Out Quad",
    "Ease In Cubic", "Ease Out Cubic", "Ease In/Out Cubic",
    "Ease In Quart", "Ease Out Quart", "Ease In/Out Quart",
    "Ease In Quint", "Ease Out Quint", "Ease In/Out Quint",
    "Ease In Sine", "Ease Out Sine", "Ease In/Out Sine",
    "Ease In Expo", "Ease Out Expo", "Ease In/Out Expo",
    "Ease In Circ", "Ease Out Circ", "Ease In/Out Circ",
    "Ease In Elastic", "Ease Out Elastic", "Ease In/Out Elastic",
    "Ease In Back", "Ease Out Back", "Ease In/Out Back",
    "Ease In Bounce", "Ease Out Bounce", "Ease In/Out Bounce"
};

void registerProperties(ecs::tint_filter_component& c, ComponentInspector& inspector) {
    inspector.addProperty("Enabled", &c.enabled);
    inspector.addProperty("Order", &c.order);
    inspector.addProperty("Color", &c.color);
    inspector.addProperty("Strength", &c.strength, 0.0f, 1.0f, 0.01f);
    inspector.addProperty("Convert to Grey", &c.convertToGrey);
}

void registerProperties(ecs::invert_filter_component& c, ComponentInspector& inspector) {
    inspector.addProperty("Enabled", &c.enabled);
    inspector.addProperty("Order", &c.order);
    inspector.addProperty("Invert RGB", &c.invertRGB);
    inspector.addProperty("Invert Alpha", &c.invertAlpha);
    inspector.addProperty("Strength", &c.strength, 0.0f, 1.0f, 0.01f);
}

void registerProperties(ecs::mirror_filter_component& c, ComponentInspector& inspector) {
    inspector.addProperty("Enabled", &c.enabled);
    inspector.addProperty("Order", &c.order);
    inspector.addProperty("Horizontal", &c.horizontal);
    inspector.addProperty("Vertical", &c.vertical);
}

void registerProperties(ecs::color_adjust_component& c, ComponentInspector& inspector) {
    inspector.addProperty("Enabled", &c.enabled);
    inspector.addProperty("Order", &c.order);
    inspector.addProperty("Brightness", &c.brightness, -1.0f, 1.0f);
    inspector.addProperty("Contrast", &c.contrast, 0.0f, 2.0f);
    inspector.addProperty("Saturation", &c.saturation, 0.0f, 2.0f);
    inspector.addProperty("Gamma", &c.gamma, 0.1f, 3.0f);
    inspector.addProperty("Hue Shift", &c.hueShift, 0.0f, 360.0f);
}

void registerProperties(ecs::blur_filter_component& c, ComponentInspector& inspector) {
    inspector.addProperty("Enabled", &c.enabled);
    inspector.addProperty("Order", &c.order);
    inspector.addProperty("Radius", &c.radius, 0.0f, 50.0f);
    inspector.addProperty("Passes", &c.passes, 1, 10);
    inspector.addProperty("Horizontal", &c.horizontal);
    inspector.addProperty("Vertical", &c.vertical);
}

void registerProperties(ecs::dither_filter_component& c, ComponentInspector& inspector) {
    inspector.addProperty("Enabled", &c.enabled);
    inspector.addProperty("Order", &c.order);
    inspector.addCustomProperty("Type", [&c]() {
        int type = (int)c.type;
        if (ImGui::Combo("##ditherType", &type, s_ditherTypeNames, IM_ARRAYSIZE(s_ditherTypeNames))) {
            c.type = (ecs::DitherType)type;
        }
    });
    inspector.addProperty("Levels", &c.levels, 2, 256);
    inspector.addProperty("Color Dither", &c.colorDither);
    inspector.addProperty("Strength", &c.strength, 0.0f, 1.0f, 0.01f);
}

void registerProperties(ecs::rotate_filter_component& c, ComponentInspector& inspector) {
    inspector.addProperty("Enabled", &c.enabled);
    inspector.addProperty("Order", &c.order);
    inspector.addProperty("90° Steps", &c.rotate90Count, 0, 3);
    inspector.addProperty("Flip Horizontal", &c.horizontalMirror);
    inspector.addProperty("Flip Vertical", &c.verticalMirror);
    inspector.addProperty("Rotate Canvas", &c.rotateCanvas);
    inspector.addProperty("Angle", &c.angleDegrees, 0.0f, 360.0f, 1.0f);
    inspector.addProperty("90 CW", &c.rotate90CW);
    inspector.addProperty("90 CCW", &c.rotate90CCW);
    inspector.addProperty("180", &c.rotate180);
}

void registerProperties(ecs::duplicate_filter_component& c, ComponentInspector& inspector) {
    inspector.addProperty("Enabled", &c.enabled);
    inspector.addProperty("Order", &c.order);
    inspector.addProperty("Horz Copies", &c.hCount, 1, 100);
    inspector.addProperty("Vert Copies", &c.vCount, 1, 100);
    inspector.addProperty("Mirror", &c.mirror);
}

void registerProperties(ecs::threshold_filter_component& c, ComponentInspector& inspector) {
    inspector.addProperty("Enabled", &c.enabled);
    inspector.addProperty("Order", &c.order);
    inspector.addProperty("Threshold", &c.threshold, 0.0f, 1.0f, 0.01f);
    inspector.addProperty("Invert", &c.invert);
    inspector.addProperty("Low Color", &c.lowColor);
    inspector.addProperty("High Color", &c.highColor);
}

void registerProperties(ecs::posterize_filter_component& c, ComponentInspector& inspector) {
    inspector.addProperty("Enabled", &c.enabled);
    inspector.addProperty("Order", &c.order);
    inspector.addProperty("Levels", &c.levels, 2, 256);
}

void registerProperties(ecs::noise_filter_component& c, ComponentInspector& inspector) {
    inspector.addProperty("Enabled", &c.enabled);
    inspector.addProperty("Order", &c.order);
    inspector.addProperty("Amount", &c.amount, 0.0f, 1.0f, 0.01f);
    inspector.addProperty("Monochrome", &c.monochrome);
    inspector.addProperty("Animated", &c.animated);
    inspector.addProperty("Seed", &c.seed);
}

void registerProperties(ecs::vignette_filter_component& c, ComponentInspector& inspector) {
    inspector.addProperty("Enabled", &c.enabled);
    inspector.addProperty("Order", &c.order);
    inspector.addProperty("Radius", &c.radius, 0.0f, 1.0f, 0.01f);
    inspector.addProperty("Softness", &c.softness, 0.0f, 1.0f, 0.01f);
    inspector.addProperty("Strength", &c.strength, 0.0f, 1.0f, 0.01f);
    inspector.addProperty("Color", &c.color);
}

void registerProperties(ecs::chromatic_aberration_component& c, ComponentInspector& inspector) {
    inspector.addProperty("Enabled", &c.enabled);
    inspector.addProperty("Order", &c.order);
    inspector.addProperty("Amount", &c.amount, 0.0f, 20.0f);
    inspector.addProperty("Angle", &c.angle, 0.0f, 360.0f);
    inspector.addProperty("Radial", &c.radial);
}

void registerProperties(ecs::edge_detect_filter_component& c, ComponentInspector& inspector) {
    inspector.addProperty("Enabled", &c.enabled);
    inspector.addProperty("Order", &c.order);
    inspector.addProperty("Strength", &c.strength, 0.0f, 1.0f, 0.01f);
}

void registerProperties(ecs::dots_generator_component& c, ComponentInspector& inspector) {
    inspector.addProperty("Enabled", &c.enabled);
    inspector.addProperty("Count X", &c.countX, 1, 100);
    inspector.addProperty("Count Y", &c.countY, 1, 100);
    inspector.addProperty("Dot Size", &c.dotSize, 0.5f, 50.0f);
    inspector.addProperty("Spacing", &c.spacing, 0.0f, 20.0f);
    inspector.addProperty("Dot Color", &c.dotColor);
    inspector.addProperty("Background", &c.bgColor);
    inspector.addProperty("Random Size", &c.randomSize);
    if (c.randomSize) {
        inspector.addProperty("Size Min", &c.randomSizeMin, 0.5f, 20.0f);
        inspector.addProperty("Size Max", &c.randomSizeMax, 1.0f, 50.0f);
    }
    inspector.addProperty("Filled", &c.filled);
    if (!c.filled) {
        inspector.addProperty("Stroke Weight", &c.strokeWeight, 0.5f, 5.0f);
    }
    inspector.addProperty("Offset Rows", &c.offset);
}

void registerProperties(ecs::stripes_generator_component& c, ComponentInspector& inspector) {
    inspector.addProperty("Enabled", &c.enabled);
    inspector.addProperty("Vertical", &c.vertical);
    inspector.addProperty("Count", &c.count, 1, 100);
    inspector.addProperty("Stripe Width", &c.stripeWidth, 0.5f, 50.0f);
    inspector.addProperty("Color 1", &c.color1);
    inspector.addProperty("Color 2", &c.color2);
    inspector.addProperty("Offset", &c.offset, 0.0f, 1.0f, 0.01f);
    inspector.addProperty("Angle", &c.angle, 0.0f, 360.0f, 1.0f);
}

void registerProperties(ecs::checkerboard_generator_component& c, ComponentInspector& inspector) {
    inspector.addProperty("Enabled", &c.enabled);
    inspector.addProperty("Count X", &c.countX, 1, 64);
    inspector.addProperty("Count Y", &c.countY, 1, 64);
    inspector.addProperty("Color 1", &c.color1);
    inspector.addProperty("Color 2", &c.color2);
}

void registerProperties(ecs::noise_generator_component& c, ComponentInspector& inspector) {
    inspector.addProperty("Enabled", &c.enabled);
    inspector.addProperty("Scale", &c.scale, 0.001f, 0.1f, 0.001f);
    inspector.addProperty("Speed", &c.speed, 0.0f, 10.0f, 0.1f);
    inspector.addProperty("Octaves", &c.octaves, 1, 8);
    inspector.addProperty("Persistence", &c.persistence, 0.0f, 1.0f, 0.01f);
    inspector.addProperty("Low Color", &c.colorLow);
    inspector.addProperty("High Color", &c.colorHigh);
}

void registerProperties(ecs::pixelate_filter_component& c, ComponentInspector& inspector) {
    inspector.addProperty("Enabled", &c.enabled);
    inspector.addProperty("Tiles X", &c.tilesX, 1, 256);
    inspector.addProperty("Tiles Y", &c.tilesY, 1, 256);
    inspector.addCustomProperty("Style", [&c]() {
        int style = (int)c.style;
        if (ImGui::Combo("##pixelStyle", &style, s_pixelStyleNames, IM_ARRAYSIZE(s_pixelStyleNames))) {
            c.style = (ecs::PixelStyle)style;
        }
    });
    inspector.addProperty("Rounded", &c.rounded, 0.0f, 1.0f, 0.01f);
    inspector.addProperty("Polka", &c.polka);
    inspector.addProperty("Gap Ratio", &c.gapRatio, 0.0f, 0.9f, 0.01f);
    if (c.style == ecs::PixelStyle::CMYK) {
        inspector.addProperty("Cyan", &c.cyanColor);
        inspector.addProperty("Magenta", &c.magentaColor);
        inspector.addProperty("Yellow", &c.yellowColor);
        inspector.addProperty("Black", &c.blackColor);
    }
}

void registerProperties(ecs::mesh_filter_component& c, ComponentInspector& inspector) {
    inspector.addProperty("Enabled", &c.enabled);
    inspector.addProperty("Grid X", &c.gridX, 1, 100);
    inspector.addProperty("Grid Y", &c.gridY, 1, 100);
    inspector.addProperty("Line Width", &c.lineWidth, 0.5f, 10.0f);
    inspector.addProperty("Vertical", &c.drawVertical);
    inspector.addProperty("Horizontal", &c.drawHorizontal);
    inspector.addProperty("Diagonal", &c.drawDiagonal);
    inspector.addProperty("Color from Image", &c.colorFromImage);
    if (!c.colorFromImage) {
        inspector.addProperty("Line Color", &c.lineColor);
    }
    inspector.addProperty("Draw Background", &c.drawBackground);
    if (c.drawBackground) {
        inspector.addProperty("Background", &c.backgroundColor);
    }
}

void registerProperties(ecs::noise_displacement_component& c, ComponentInspector& inspector) {
    inspector.addProperty("Enabled", &c.enabled);
    inspector.addProperty("Scale", &c.scale, 0.001f, 0.5f, 0.001f);
    inspector.addProperty("Amplitude", &c.amplitude, 0.f, 100.f, 0.5f);
    inspector.addProperty("Speed", &c.speed, 0.f, 10.f, 0.1f);
    inspector.addProperty("Time Offset", &c.timeOffset, 0.f, 100.f, 0.1f);
}

void registerProperties(ecs::rings_filter_component& c, ComponentInspector& inspector) {
    inspector.addProperty("Enabled", &c.enabled);
    inspector.addProperty("Ring Count", &c.ringCount, 1, 100);
    inspector.addProperty("Line Width", &c.lineWidth, 0.5f, 10.0f);
    inspector.addProperty("Color from Image", &c.colorFromImage);
    if (!c.colorFromImage) {
        inspector.addProperty("Ring Color", &c.ringColor);
    }
    inspector.addCustomProperty("Center", [&c]() {
        float center[2] = { c.center.x, c.center.y };
        if (ImGui::SliderFloat2("##center", center, 0.0f, 1.0f)) {
            c.center.x = center[0];
            c.center.y = center[1];
        }
    });
    inspector.addProperty("Filled", &c.filled);
    inspector.addProperty("Gap Ratio", &c.gapRatio, 0.0f, 1.0f, 0.01f);
}

void registerProperties(ecs::line_scan_filter_component& c, ComponentInspector& inspector) {
    inspector.addProperty("Enabled", &c.enabled);
    inspector.addProperty("Vertical", &c.vertical);
    inspector.addProperty("Line Count", &c.lineCount, 1, 500);
    inspector.addProperty("Line Width", &c.lineWidth, 0.5f, 10.0f);
    inspector.addProperty("Color from Image", &c.colorFromImage);
    if (!c.colorFromImage) {
        inspector.addProperty("Line Color", &c.lineColor);
    }
    inspector.addProperty("Background", &c.backgroundColor);
    inspector.addProperty("Size from Brightness", &c.sizeFromBrightness);
    if (c.sizeFromBrightness) {
        inspector.addProperty("Min Width", &c.minLineWidth, 0.1f, 5.0f);
        inspector.addProperty("Max Width", &c.maxLineWidth, 1.0f, 10.0f);
    }
}

void registerProperties(ecs::ascii_filter_component& c, ComponentInspector& inspector) {
    inspector.addProperty("Enabled", &c.enabled);
    inspector.addProperty("Chars X", &c.charsX, 10, 200);
    inspector.addProperty("Chars Y", &c.charsY, 5, 100);
    inspector.addProperty("Font Size", &c.fontSize, 4.0f, 24.0f);
    inspector.addProperty("Color from Image", &c.colorFromImage);
    if (!c.colorFromImage) {
        inspector.addProperty("Text Color", &c.textColor);
    }
    inspector.addProperty("Background", &c.backgroundColor);
    inspector.addProperty("Invert", &c.invert);
    inspector.addCustomProperty("Charset", [&c]() {
        char buf[64];
        strncpy(buf, c.charset.c_str(), sizeof(buf) - 1);
        buf[sizeof(buf) - 1] = '\0';
        if (ImGui::InputText("##charset", buf, sizeof(buf))) {
            c.charset = buf;
        }
    });
}

void registerProperties(ecs::eased_pulse_component& c, ComponentInspector& inspector) {
    inspector.addProperty("Playing", &c.playing);
    inspector.addProperty("Frequency", &c.frequency, 0.01f, 10.0f);
    inspector.addProperty("Duty Cycle", &c.dutyCycle, 0.01f, 0.99f);
    inspector.addProperty("Rise Time", &c.riseTime, 0.01f, 2.0f);
    inspector.addProperty("Fall Time", &c.fallTime, 0.01f, 2.0f);
    inspector.addCustomProperty("Rise Easing", [&c]() {
        int type = (int)c.riseEasing;
        if (ImGui::Combo("##riseEase", &type, s_easingTypeNames, IM_ARRAYSIZE(s_easingTypeNames))) {
            c.riseEasing = (ecs::EasingType)type;
        }
    });
    inspector.addCustomProperty("Fall Easing", [&c]() {
        int type = (int)c.fallEasing;
        if (ImGui::Combo("##fallEase", &type, s_easingTypeNames, IM_ARRAYSIZE(s_easingTypeNames))) {
            c.fallEasing = (ecs::EasingType)type;
        }
    });
    inspector.addProperty("Min Value", &c.minValue);
    inspector.addProperty("Max Value", &c.maxValue);
    inspector.addCustomProperty("Value", [&c]() {
        ImGui::ProgressBar(c.getNormalizedValue(), ImVec2(-1, 0), "");
        ImGui::SameLine();
        ImGui::Text("%.2f", c.value);
    });
    inspector.addCustomProperty("Controls", [&c]() {
        if (ImGui::Button("Play")) c.play();
        ImGui::SameLine();
        if (ImGui::Button("Pause")) c.pause();
        ImGui::SameLine();
        if (ImGui::Button("Reset")) c.reset();
    });
    inspector.addCustomProperty("Bindings", [&c]() {
        ImGui::Text("%d bindings", (int)c.bindings.size());
    });
}

void registerProperties(ecs::state_preset_component& c, ComponentInspector& inspector) {
    inspector.addProperty("Name", &c.name);
    inspector.addProperty("Preview Color", &c.previewColor);
    inspector.addCustomProperty("Info", [&c]() {
        ImGui::Text("Entities: %d", c.getEntityCount());
        ImGui::Text("Properties: %d", c.getPropertyCount());
    });
    inspector.addCustomProperty("Actions", [&c]() {
        if (ImGui::Button("Clear")) {
            c.clear();
        }
    });
}

void registerProperties(ecs::state_library_component& c, ComponentInspector& inspector) {
    inspector.addProperty("Name", &c.name);
    inspector.addCustomProperty("Presets", [&c]() {
        ImGui::Text("%d presets", c.getPresetCount());
        for (int i = 0; i < c.getPresetCount(); i++) {
            ImGui::PushID(i);
            auto* preset = c.getPreset(i);
            bool selected = (c.currentPresetIndex == i);
            ImVec4 color(preset->previewColor.r / 255.0f, preset->previewColor.g / 255.0f,
                        preset->previewColor.b / 255.0f, preset->previewColor.a / 255.0f);
            ImGui::ColorButton("##col", color, ImGuiColorEditFlags_NoTooltip, ImVec2(16, 16));
            ImGui::SameLine();
            if (ImGui::Selectable(preset->name.c_str(), selected)) {
                c.currentPresetIndex = i;
            }
            ImGui::PopID();
        }
        if (ImGui::Button("+ Add Preset")) {
            c.addPreset("State " + std::to_string(c.getPresetCount() + 1));
        }
    });
}

void registerProperties(ecs::state_timeline_component& c, ComponentInspector& inspector) {
    inspector.addProperty("Duration", &c.duration, 1.0f, 120.0f);
    inspector.addProperty("Loop", &c.loop);
    inspector.addProperty("Playback Speed", &c.playbackSpeed, 0.1f, 4.0f);
    inspector.addCustomProperty("Playhead", [&c]() {
        float playhead = c.playhead;
        if (ImGui::SliderFloat("##playhead", &playhead, 0.0f, c.getEffectiveDuration())) {
            c.setPlayhead(playhead);
        }
    });
    inspector.addProperty("Sync to BPM", &c.syncToBPM);
    if (c.syncToBPM) {
        inspector.addProperty("BPM", &c.bpm, 20.0f, 300.0f);
        inspector.addProperty("Beats per Loop", &c.beatsPerLoop, 1, 32);
    }
    inspector.addCustomProperty("Controls", [&c]() {
        if (ImGui::Button(c.playing ? "Pause" : "Play")) {
            if (c.playing) c.pause();
            else c.play();
        }
        ImGui::SameLine();
        if (ImGui::Button("Stop")) {
            c.stop();
        }
    });
    inspector.addCustomProperty("Keyframes", [&c]() {
        ImGui::Text("%d keyframes", c.getKeyframeCount());
    });
}

void registerProperties(ecs::state_morph_component& c, ComponentInspector& inspector) {
    inspector.addProperty("Active", &c.active);
    inspector.addProperty("Duration", &c.duration, 0.1f, 10.0f);
    inspector.addCustomProperty("Easing", [&c]() {
        int type = (int)c.easing;
        if (ImGui::Combo("##morphEase", &type, s_easingTypeNames, IM_ARRAYSIZE(s_easingTypeNames))) {
            c.easing = (ecs::EasingType)type;
        }
    });
    inspector.addCustomProperty("Progress", [&c]() {
        ImGui::ProgressBar(c.progress, ImVec2(-1, 0), "");
        ImGui::SameLine();
        ImGui::Text("%.1f%%", c.progress * 100.0f);
    });
    inspector.addCustomProperty("Controls", [&c]() {
        if (ImGui::Button("Cancel")) {
            c.cancel();
        }
        ImGui::SameLine();
        if (ImGui::Button("Reset")) {
            c.reset();
        }
    });
}

void registerProperties(ecs::canvas_effects_component& c, ComponentInspector& inspector) {
    inspector.addProperty("Auto Apply",    &c.autoApply);
    inspector.addProperty("Use Ping-Pong", &c.usePingPong);
    inspector.addCustomProperty("Effect Count", [&c] {
        ImGui::Text("Effects in stack: %d", c.getEffectCount());
    });
}

} // namespace inspector
