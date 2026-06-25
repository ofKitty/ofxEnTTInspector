// ============================================================================
// Graphics 2D Component Inspectors - Implementation
// ============================================================================

#include "graphics2d_inspectors.h"
#include <cstring>

namespace inspector {

// ============================================================================
// UNIFIED Shape2D Component Inspector
// ============================================================================

void registerProperties(ecs::shape2d_component& comp, ComponentInspector& inspector) {
    inspector.addCustomProperty("Shape Type", [&]() {
        ImGui::Text("Type: %s", ecs::getShapeTypeName(comp.type));
    });
    
    bool showFillStroke = (comp.type != ecs::Shape2DType::Line && 
                           comp.type != ecs::Shape2DType::Grid &&
                           comp.type != ecs::Shape2DType::FlowerOfLife &&
                           comp.type != ecs::Shape2DType::MetatronsCube);
    
    if (showFillStroke) {
        inspector.addProperty("Filled", &comp.filled);
        if (comp.filled) {
            inspector.addProperty("Fill Color", &comp.fillColor);
        }
        inspector.addProperty("Stroked", &comp.stroked);
        if (comp.stroked) {
            inspector.addProperty("Stroke Color", &comp.strokeColor);
            inspector.addProperty("Stroke Width", &comp.strokeWidth, 0.1f, 50.0f);
        }
    }
    
    switch (comp.type) {
        case ecs::Shape2DType::Rectangle:
            inspector.addCustomProperty("Position", [&]() {
                ImGui::DragFloat("X", &comp.x, 1.0f);
                ImGui::DragFloat("Y", &comp.y, 1.0f);
            });
            inspector.addCustomProperty("Size", [&]() {
                ImGui::DragFloat("Width", &comp.width, 1.0f, 1.0f, 10000.0f);
                ImGui::DragFloat("Height", &comp.height, 1.0f, 1.0f, 10000.0f);
            });
            inspector.addProperty("Corner Radius", &comp.cornerRadius, 0.0f, 500.0f);
            break;
            
        case ecs::Shape2DType::Circle:
            inspector.addCustomProperty("Position", [&]() {
                ImGui::DragFloat("X", &comp.x, 1.0f);
                ImGui::DragFloat("Y", &comp.y, 1.0f);
            });
            inspector.addProperty("Radius", &comp.radius, 1.0f, 10000.0f);
            inspector.addProperty("Resolution", &comp.resolution, 3, 128);
            break;
            
        case ecs::Shape2DType::Ellipse:
            inspector.addCustomProperty("Position", [&]() {
                ImGui::DragFloat("X", &comp.x, 1.0f);
                ImGui::DragFloat("Y", &comp.y, 1.0f);
            });
            inspector.addCustomProperty("Radii", [&]() {
                ImGui::DragFloat("Radius X", &comp.innerRadius, 1.0f, 1.0f, 10000.0f);
                ImGui::DragFloat("Radius Y", &comp.outerRadius, 1.0f, 1.0f, 10000.0f);
            });
            inspector.addProperty("Resolution", &comp.resolution, 3, 128);
            break;
            
        case ecs::Shape2DType::Line:
            inspector.addCustomProperty("Start", [&]() {
                ImGui::DragFloat2("Start", &comp.start.x, 1.0f);
            });
            inspector.addCustomProperty("End", [&]() {
                ImGui::DragFloat2("End", &comp.end.x, 1.0f);
            });
            inspector.addProperty("Color", &comp.color);
            inspector.addProperty("Line Width", &comp.lineWidth, 0.1f, 50.0f);
            break;
            
        case ecs::Shape2DType::Triangle:
            inspector.addCustomProperty("Points", [&]() {
                ImGui::DragFloat2("P1", &comp.p0.x, 1.0f);
                ImGui::DragFloat2("P2", &comp.p1.x, 1.0f);
                ImGui::DragFloat2("P3", &comp.p2.x, 1.0f);
            });
            break;
            
        case ecs::Shape2DType::Arc:
            inspector.addCustomProperty("Position", [&]() {
                ImGui::DragFloat("X", &comp.x, 1.0f);
                ImGui::DragFloat("Y", &comp.y, 1.0f);
            });
            inspector.addProperty("Radius", &comp.radius, 1.0f, 10000.0f);
            inspector.addCustomProperty("Angles", [&]() {
                ImGui::DragFloat("Start Angle", &comp.angleStart, 1.0f, 0.0f, 360.0f);
                ImGui::DragFloat("End Angle", &comp.angleEnd, 1.0f, 0.0f, 360.0f);
            });
            inspector.addProperty("Resolution", &comp.resolution, 3, 128);
            break;
            
        case ecs::Shape2DType::Star:
            inspector.addCustomProperty("Position", [&]() {
                ImGui::DragFloat("X", &comp.x, 1.0f);
                ImGui::DragFloat("Y", &comp.y, 1.0f);
            });
            inspector.addProperty("Points", &comp.numPoints, 3, 20);
            inspector.addProperty("Inner Radius", &comp.innerRadius, 1.0f, 500.0f);
            inspector.addProperty("Outer Radius", &comp.outerRadius, 1.0f, 500.0f);
            break;
            
        case ecs::Shape2DType::Ring:
            inspector.addCustomProperty("Position", [&]() {
                ImGui::DragFloat("X", &comp.x, 1.0f);
                ImGui::DragFloat("Y", &comp.y, 1.0f);
            });
            inspector.addProperty("Inner Radius", &comp.innerRadius, 1.0f, 500.0f);
            inspector.addProperty("Outer Radius", &comp.outerRadius, 1.0f, 500.0f);
            inspector.addProperty("Resolution", &comp.resolution, 8, 128);
            break;
            
        case ecs::Shape2DType::Cross:
            inspector.addCustomProperty("Position", [&]() {
                ImGui::DragFloat("X", &comp.x, 1.0f);
                ImGui::DragFloat("Y", &comp.y, 1.0f);
            });
            inspector.addProperty("Width", &comp.width, 1.0f, 500.0f);
            inspector.addProperty("Height", &comp.height, 1.0f, 500.0f);
            inspector.addProperty("Thickness", &comp.thickness, 1.0f, 200.0f);
            break;
            
        case ecs::Shape2DType::Heart:
            inspector.addCustomProperty("Position", [&]() {
                ImGui::DragFloat("X", &comp.x, 1.0f);
                ImGui::DragFloat("Y", &comp.y, 1.0f);
            });
            inspector.addProperty("Size", &comp.size, 1.0f, 500.0f);
            inspector.addProperty("Resolution", &comp.resolution, 8, 128);
            break;
            
        case ecs::Shape2DType::Arrow:
            inspector.addCustomProperty("Start", [&]() {
                ImGui::DragFloat2("Start", &comp.start.x, 1.0f);
            });
            inspector.addCustomProperty("End", [&]() {
                ImGui::DragFloat2("End", &comp.end.x, 1.0f);
            });
            inspector.addProperty("Color", &comp.color);
            inspector.addProperty("Line Width", &comp.lineWidth, 0.1f, 50.0f);
            inspector.addProperty("Head Length", &comp.headLength, 1.0f, 100.0f);
            inspector.addProperty("Head Angle", &comp.headAngle, 0.1f, 1.5f);
            inspector.addProperty("Double Ended", &comp.doubleEnded);
            break;
            
        case ecs::Shape2DType::RegularPolygon:
            inspector.addCustomProperty("Position", [&]() {
                ImGui::DragFloat("X", &comp.x, 1.0f);
                ImGui::DragFloat("Y", &comp.y, 1.0f);
            });
            inspector.addProperty("Sides", &comp.numSides, 3, 20);
            inspector.addProperty("Radius", &comp.radius, 1.0f, 500.0f);
            inspector.addProperty("Rotation", &comp.rotation, 0.0f, 360.0f);
            break;
            
        case ecs::Shape2DType::BezierCurve:
            inspector.addCustomProperty("Control Points", [&]() {
                ImGui::DragFloat2("P0 (Start)", &comp.p0.x, 1.0f);
                ImGui::DragFloat2("P1 (Control 1)", &comp.p1.x, 1.0f);
                ImGui::DragFloat2("P2 (Control 2)", &comp.p2.x, 1.0f);
                ImGui::DragFloat2("P3 (End)", &comp.p3.x, 1.0f);
            });
            inspector.addProperty("Resolution", &comp.resolution, 5, 200);
            inspector.addProperty("Color", &comp.color);
            inspector.addProperty("Line Width", &comp.lineWidth, 0.1f, 50.0f);
            inspector.addProperty("Show Control Points", &comp.showControlPoints);
            break;
            
        case ecs::Shape2DType::Spline:
            inspector.addProperty("Tension", &comp.tension, 0.0f, 1.0f);
            inspector.addProperty("Resolution", &comp.resolution, 1, 50);
            inspector.addProperty("Closed", &comp.closed);
            inspector.addProperty("Color", &comp.color);
            inspector.addProperty("Line Width", &comp.lineWidth, 0.1f, 50.0f);
            inspector.addProperty("Show Control Points", &comp.showControlPoints);
            inspector.addCustomProperty("Stats", [&]() {
                ImGui::Text("Control Points: %zu", comp.controlPoints.size());
            });
            break;
            
        case ecs::Shape2DType::Path:
            inspector.addProperty("Curve Resolution", &comp.curveResolution, 5, 200);
            inspector.addProperty("Closed", &comp.closed);
            inspector.addCustomProperty("Operations", [&]() {
                if (ImGui::Button("Close Path")) comp.pathClose();
                ImGui::SameLine();
                if (ImGui::Button("Clear")) comp.pathClear();
            });
            break;
            
        case ecs::Shape2DType::Polyline:
            inspector.addProperty("Color", &comp.color);
            inspector.addProperty("Line Width", &comp.lineWidth, 0.1f, 50.0f);
            inspector.addProperty("Closed", &comp.closed);
            inspector.addProperty("Smoothed", &comp.smoothed);
            if (comp.smoothed) {
                inspector.addProperty("Smoothing Size", &comp.smoothingSize, 1.0f, 50.0f);
                inspector.addProperty("Smoothing Shape", &comp.smoothingShape, 0.0f, 1.0f);
            }
            inspector.addCustomProperty("Stats", [&]() {
                ImGui::Text("Vertices: %zu", comp.polyline.size());
            });
            break;
            
        case ecs::Shape2DType::Gradient:
            inspector.addProperty("Color Start", &comp.colorStart);
            inspector.addProperty("Color End", &comp.colorEnd);
            inspector.addCustomProperty("Type", [&]() {
                int type = (int)comp.gradientType;
                if (ImGui::Combo("Gradient Type", &type, "Linear\0Radial\0")) {
                    comp.gradientType = (ecs::shape2d_component::GradientType)type;
                    comp.needsRebuild = true;
                }
            });
            if (comp.gradientType == ecs::shape2d_component::GRADIENT_LINEAR) {
                inspector.addProperty("Angle", &comp.angle, 0.0f, 360.0f);
            } else {
                inspector.addCustomProperty("Center", [&]() {
                    ImGui::DragFloat2("Center", &comp.center.x, 1.0f);
                });
            }
            inspector.addProperty("Width", &comp.width, 1.0f, 5000.0f);
            inspector.addProperty("Height", &comp.height, 1.0f, 5000.0f);
            break;
            
        case ecs::Shape2DType::Grid:
            inspector.addProperty("Width", &comp.width, 1.0f, 10000.0f);
            inspector.addProperty("Height", &comp.height, 1.0f, 10000.0f);
            inspector.addProperty("Major Spacing", &comp.majorSpacing, 1.0f, 1000.0f);
            inspector.addProperty("Minor Spacing", &comp.minorSpacing, 1.0f, 500.0f);
            inspector.addProperty("Major Line Color", &comp.majorLineColor);
            inspector.addProperty("Minor Line Color", &comp.minorLineColor);
            inspector.addProperty("Draw Major Lines", &comp.drawMajorLines);
            inspector.addProperty("Draw Minor Lines", &comp.drawMinorLines);
            inspector.addProperty("Draw Axes", &comp.drawAxes);
            break;
            
        case ecs::Shape2DType::ProgressBar:
            inspector.addProperty("Value", &comp.value, 0.0f, 1.0f);
            inspector.addProperty("Width", &comp.width, 1.0f, 1000.0f);
            inspector.addProperty("Height", &comp.height, 1.0f, 200.0f);
            inspector.addProperty("Corner Radius", &comp.cornerRadius, 0.0f, 50.0f);
            inspector.addProperty("Background Color", &comp.backgroundColor);
            inspector.addProperty("Border Color", &comp.borderColor);
            inspector.addProperty("Show Background", &comp.showBackground);
            inspector.addProperty("Show Border", &comp.showBorder);
            inspector.addProperty("Show Text", &comp.showText);
            break;
            
        case ecs::Shape2DType::Text2D:
            inspector.addCustomProperty("Text", [&]() {
                static char textBuffer[1024];
                strncpy(textBuffer, comp.text.c_str(), sizeof(textBuffer) - 1);
                if (ImGui::InputTextMultiline("##text", textBuffer, sizeof(textBuffer), ImVec2(-1, 60))) {
                    comp.text = textBuffer;
                }
            });
            inspector.addProperty("Font Size", &comp.fontSize, 8, 200);
            inspector.addProperty("Color", &comp.color);
            inspector.addProperty("Line Height", &comp.lineHeight, 0.5f, 3.0f);
            inspector.addProperty("Letter Spacing", &comp.letterSpacing, -0.5f, 2.0f);
            inspector.addProperty("Word Wrap", &comp.wordWrap);
            if (comp.wordWrap) {
                inspector.addProperty("Wrap Width", &comp.wrapWidth, 50.0f, 2000.0f);
            }
            break;
            
        case ecs::Shape2DType::VesicaPiscis:
            inspector.addCustomProperty("Position", [&]() {
                ImGui::DragFloat("X", &comp.x, 1.0f);
                ImGui::DragFloat("Y", &comp.y, 1.0f);
            });
            inspector.addProperty("Width", &comp.width, 1.0f, 10000.0f);
            inspector.addProperty("Height", &comp.height, 1.0f, 10000.0f);
            inspector.addProperty("Resolution", &comp.resolution, 8, 128);
            inspector.addProperty("Show Construction", &comp.showConstruction);
            break;
            
        case ecs::Shape2DType::FlowerOfLife:
            inspector.addCustomProperty("Position", [&]() {
                ImGui::DragFloat("X", &comp.x, 1.0f);
                ImGui::DragFloat("Y", &comp.y, 1.0f);
            });
            inspector.addProperty("Radius", &comp.radius, 1.0f, 500.0f);
            inspector.addProperty("Rings", &comp.rings, 1, 10);
            inspector.addProperty("Resolution", &comp.resolution, 8, 128);
            inspector.addProperty("Color", &comp.color);
            inspector.addProperty("Line Width", &comp.lineWidth, 0.1f, 10.0f);
            inspector.addProperty("Show Outer Circle", &comp.showOuterCircle);
            break;
            
        case ecs::Shape2DType::MetatronsCube:
            inspector.addCustomProperty("Position", [&]() {
                ImGui::DragFloat("X", &comp.x, 1.0f);
                ImGui::DragFloat("Y", &comp.y, 1.0f);
            });
            inspector.addProperty("Inner Radius", &comp.innerRadius, 1.0f, 500.0f);
            inspector.addProperty("Outer Radius", &comp.outerRadius, 1.0f, 500.0f);
            inspector.addProperty("Color", &comp.color);
            inspector.addProperty("Line Width", &comp.lineWidth, 0.1f, 10.0f);
            inspector.addProperty("Show Circles", &comp.showCircles);
            break;
            
        default:
            inspector.addCustomProperty("Info", [&]() {
                ImGui::Text("No specific properties for this shape type");
            });
            break;
    }
}

// ============================================================================
// Path Component Inspector
// ============================================================================

void registerProperties(ecs::path_component& comp, ComponentInspector& inspector) {
    inspector.addProperty("Fill Color", &comp.fillColor);
    inspector.addProperty("Stroke Color", &comp.strokeColor);
    inspector.addProperty("Stroke Width", &comp.strokeWidth, 0.0f, 50.0f);
    
    inspector.addProperty("Filled", &comp.filled);
    inspector.addProperty("Stroked", &comp.stroked);
    inspector.addProperty("Closed", &comp.closed);
    
    inspector.addCustomProperty("Path Operations", [&]() {
        if (ImGui::Button("Close Path")) {
            comp.closePath();
        }
        ImGui::SameLine();
        if (ImGui::Button("Clear")) {
            comp.clear();
        }
        
        static float simplifyTolerance = 0.3f;
        ImGui::DragFloat("Simplify Tolerance", &simplifyTolerance, 0.01f, 0.01f, 10.0f);
        if (ImGui::Button("Simplify")) {
            comp.simplify(simplifyTolerance);
        }
    });
    
    inspector.addCustomProperty("Add Shapes", [&]() {
        static float circleRadius = 50.0f;
        static float rectW = 100.0f, rectH = 100.0f;
        
        ImGui::Text("Quick Shapes:");
        
        ImGui::DragFloat("Circle Radius", &circleRadius, 1.0f, 1.0f, 500.0f);
        if (ImGui::Button("Add Circle")) {
            comp.addCircle(glm::vec3(0, 0, 0), circleRadius);
        }
        
        ImGui::DragFloat("Rect Width", &rectW, 1.0f, 1.0f, 1000.0f);
        ImGui::DragFloat("Rect Height", &rectH, 1.0f, 1.0f, 1000.0f);
        if (ImGui::Button("Add Rectangle")) {
            comp.addRectangle(0, 0, rectW, rectH);
        }
    });
}

// ============================================================================
// Polyline Component Inspector
// ============================================================================

void registerProperties(ecs::polyline_component& comp, ComponentInspector& inspector) {
    inspector.addProperty("Color", &comp.color);
    inspector.addProperty("Line Width", &comp.lineWidth, 0.1f, 50.0f);
    inspector.addProperty("Closed", &comp.closed);
    inspector.addProperty("Smoothed", &comp.smoothed);
    
    if (comp.smoothed) {
        inspector.addProperty("Smoothing Size", &comp.smoothingSize, 1.0f, 50.0f);
        inspector.addProperty("Smoothing Shape", &comp.smoothingShape, 0.0f, 1.0f);
    }
    
    inspector.addCustomProperty("Stats", [&]() {
        ImGui::Text("Vertices: %zu", comp.size());
        ImGui::Text("Perimeter: %.2f", comp.getPerimeter());
        
        auto bbox = comp.getBoundingBox();
        ImGui::Text("Bounds: (%.1f, %.1f) - (%.1f, %.1f)", 
            bbox.x, bbox.y, bbox.x + bbox.width, bbox.y + bbox.height);
    });
    
    inspector.addCustomProperty("Operations", [&]() {
        if (ImGui::Button(comp.closed ? "Open" : "Close")) {
            comp.setClosed(!comp.closed);
        }
        ImGui::SameLine();
        if (ImGui::Button("Clear")) {
            comp.clear();
        }
        
        static float simplifyTolerance = 0.3f;
        ImGui::DragFloat("Simplify Tolerance", &simplifyTolerance, 0.01f, 0.01f, 10.0f);
        if (ImGui::Button("Simplify")) {
            comp.simplify(simplifyTolerance);
        }
        
        if (ImGui::Button("Smooth")) {
            comp.smooth();
        }
    });
}

// ============================================================================
// Rectangle Component Inspector
// ============================================================================

void registerProperties(ecs::rectangle_component& comp, ComponentInspector& inspector) {
    inspector.addCustomProperty("Position", [&]() {
        ImGui::DragFloat("X", &comp.x, 1.0f);
        ImGui::DragFloat("Y", &comp.y, 1.0f);
    });
    
    inspector.addCustomProperty("Size", [&]() {
        ImGui::DragFloat("Width", &comp.width, 1.0f, 1.0f, 10000.0f);
        ImGui::DragFloat("Height", &comp.height, 1.0f, 1.0f, 10000.0f);
    });
    
    inspector.addProperty("Corner Radius", &comp.cornerRadius, 0.0f, 500.0f);
    
    inspector.addCustomProperty("Rect Mode", [&]() {
        int mode = (comp.rectMode == OF_RECTMODE_CENTER) ? 1 : 0;
        if (ImGui::Combo("Mode", &mode, "Corner\0Center\0")) {
            comp.rectMode = (mode == 1) ? OF_RECTMODE_CENTER : OF_RECTMODE_CORNER;
        }
    });
    
    inspector.addProperty("Filled", &comp.filled);
    if (comp.filled) {
        inspector.addProperty("Fill Color", &comp.fillColor);
    }
    
    inspector.addProperty("Stroked", &comp.stroked);
    if (comp.stroked) {
        inspector.addProperty("Stroke Color", &comp.strokeColor);
        inspector.addProperty("Stroke Width", &comp.strokeWidth, 0.1f, 50.0f);
    }
    
    inspector.addCustomProperty("Info", [&]() {
        auto center = comp.getCenter();
        ImGui::Text("Center: (%.1f, %.1f)", center.x, center.y);
        auto rect = comp.getRect();
        ImGui::Text("Bounds: (%.1f, %.1f) - (%.1f, %.1f)", 
            rect.x, rect.y, rect.x + rect.width, rect.y + rect.height);
    });
}

// ============================================================================
// Circle Component Inspector
// ============================================================================

void registerProperties(ecs::circle_component& comp, ComponentInspector& inspector) {
    inspector.addCustomProperty("Position", [&]() {
        ImGui::DragFloat("X", &comp.x, 1.0f);
        ImGui::DragFloat("Y", &comp.y, 1.0f);
    });
    
    inspector.addProperty("Radius", &comp.radius, 1.0f, 10000.0f);
    inspector.addProperty("Resolution", &comp.resolution, 3, 128);
    
    inspector.addProperty("Filled", &comp.filled);
    if (comp.filled) {
        inspector.addProperty("Fill Color", &comp.fillColor);
    }
    
    inspector.addProperty("Stroked", &comp.stroked);
    if (comp.stroked) {
        inspector.addProperty("Stroke Color", &comp.strokeColor);
        inspector.addProperty("Stroke Width", &comp.strokeWidth, 0.1f, 50.0f);
    }
    
    inspector.addCustomProperty("Info", [&]() {
        ImGui::Text("Diameter: %.1f", comp.radius * 2);
        ImGui::Text("Circumference: %.1f", comp.radius * TWO_PI);
        ImGui::Text("Area: %.1f", PI * comp.radius * comp.radius);
    });
}

// ============================================================================
// Ellipse Component Inspector
// ============================================================================

void registerProperties(ecs::ellipse_component& comp, ComponentInspector& inspector) {
    inspector.addCustomProperty("Position", [&]() {
        ImGui::DragFloat("X", &comp.x, 1.0f);
        ImGui::DragFloat("Y", &comp.y, 1.0f);
    });
    
    inspector.addCustomProperty("Radii", [&]() {
        ImGui::DragFloat("Radius X", &comp.radiusX, 1.0f, 1.0f, 10000.0f);
        ImGui::DragFloat("Radius Y", &comp.radiusY, 1.0f, 1.0f, 10000.0f);
    });
    
    inspector.addProperty("Resolution", &comp.resolution, 3, 128);
    
    inspector.addProperty("Filled", &comp.filled);
    if (comp.filled) {
        inspector.addProperty("Fill Color", &comp.fillColor);
    }
    
    inspector.addProperty("Stroked", &comp.stroked);
    if (comp.stroked) {
        inspector.addProperty("Stroke Color", &comp.strokeColor);
        inspector.addProperty("Stroke Width", &comp.strokeWidth, 0.1f, 50.0f);
    }
}

// ============================================================================
// Line Component Inspector
// ============================================================================

void registerProperties(ecs::line_component& comp, ComponentInspector& inspector) {
    // Drag both endpoints together (reference = start), so a line has a single
    // Position like the other 2D shapes — Start/End below still edit each end.
    inspector.addCustomProperty("Position", [&]() {
        glm::vec2 c  = comp.start;
        glm::vec2 nc = c;
        if (ImGui::DragFloat2("Position", &nc.x, 1.0f)) {
            const glm::vec2 d = nc - c;
            comp.start += d; comp.end += d;
        }
    });

    inspector.addCustomProperty("Start", [&]() {
        ImGui::DragFloat2("Start", &comp.start.x, 1.0f);
    });
    
    inspector.addCustomProperty("End", [&]() {
        ImGui::DragFloat2("End", &comp.end.x, 1.0f);
    });
    
    inspector.addProperty("Color", &comp.color);
    inspector.addProperty("Line Width", &comp.lineWidth, 0.1f, 50.0f);
    
    inspector.addProperty("Arrow", &comp.arrow);
    if (comp.arrow) {
        inspector.addProperty("Arrow Size", &comp.arrowSize, 1.0f, 100.0f);
        inspector.addProperty("Arrow Angle", &comp.arrowAngle, 10.0f, 60.0f);
    }
    
    inspector.addCustomProperty("Info", [&]() {
        ImGui::Text("Length: %.2f", comp.getLength());
        auto mid = comp.getMidpoint();
        ImGui::Text("Midpoint: (%.1f, %.1f)", mid.x, mid.y);
    });
}

// ============================================================================
// Triangle Component Inspector
// ============================================================================

void registerProperties(ecs::triangle_component& comp, ComponentInspector& inspector) {
    // Triangles store three absolute vertices (no x/y like rect/circle), so
    // expose a Position that drags all three points together — keeping parity
    // with the other 2D shapes. Reference point is the centroid.
    inspector.addCustomProperty("Position", [&]() {
        glm::vec2 c  = comp.getCentroid();
        glm::vec2 nc = c;
        if (ImGui::DragFloat2("Position", &nc.x, 1.0f)) {
            const glm::vec2 d = nc - c;
            comp.p1 += d; comp.p2 += d; comp.p3 += d;
        }
    });

    inspector.addCustomProperty("Point 1", [&]() {
        ImGui::DragFloat2("P1", &comp.p1.x, 1.0f);
    });
    
    inspector.addCustomProperty("Point 2", [&]() {
        ImGui::DragFloat2("P2", &comp.p2.x, 1.0f);
    });
    
    inspector.addCustomProperty("Point 3", [&]() {
        ImGui::DragFloat2("P3", &comp.p3.x, 1.0f);
    });
    
    inspector.addProperty("Filled", &comp.filled);
    if (comp.filled) {
        inspector.addProperty("Fill Color", &comp.fillColor);
    }
    
    inspector.addProperty("Stroked", &comp.stroked);
    if (comp.stroked) {
        inspector.addProperty("Stroke Color", &comp.strokeColor);
        inspector.addProperty("Stroke Width", &comp.strokeWidth, 0.1f, 50.0f);
    }
    
    inspector.addCustomProperty("Info", [&]() {
        auto centroid = comp.getCentroid();
        ImGui::Text("Centroid: (%.1f, %.1f)", centroid.x, centroid.y);
        ImGui::Text("Area: %.2f", comp.getArea());
    });
}

// ============================================================================
// Polygon Component Inspector
// ============================================================================

void registerProperties(ecs::polygon_component& comp, ComponentInspector& inspector) {
    inspector.addProperty("Filled", &comp.filled);
    if (comp.filled) {
        inspector.addProperty("Fill Color", &comp.fillColor);
    }
    
    inspector.addProperty("Stroked", &comp.stroked);
    if (comp.stroked) {
        inspector.addProperty("Stroke Color", &comp.strokeColor);
        inspector.addProperty("Stroke Width", &comp.strokeWidth, 0.1f, 50.0f);
    }
    
    inspector.addProperty("Convex", &comp.convex);
    
    inspector.addCustomProperty("Stats", [&]() {
        ImGui::Text("Vertices: %zu", comp.size());
        auto centroid = comp.getCentroid();
        ImGui::Text("Centroid: (%.1f, %.1f)", centroid.x, centroid.y);
        auto bbox = comp.getBoundingBox();
        ImGui::Text("Bounds: %.1f x %.1f", bbox.width, bbox.height);
    });
    
    inspector.addCustomProperty("Generate Regular", [&]() {
        static int numSides = 6;
        static float radius = 50.0f;
        
        ImGui::DragInt("Sides", &numSides, 1, 3, 20);
        ImGui::DragFloat("Radius", &radius, 1.0f, 1.0f, 500.0f);
        
        if (ImGui::Button("Generate")) {
            comp.setRegular(numSides, radius);
        }
        
        ImGui::SameLine();
        if (ImGui::Button("Clear")) {
            comp.clear();
        }
    });
}

// ============================================================================
// Arc Component Inspector
// ============================================================================

void registerProperties(ecs::arc_component& comp, ComponentInspector& inspector) {
    inspector.addCustomProperty("Position", [&]() {
        ImGui::DragFloat("X", &comp.x, 1.0f);
        ImGui::DragFloat("Y", &comp.y, 1.0f);
    });
    
    inspector.addProperty("Radius", &comp.radius, 1.0f, 10000.0f);
    
    inspector.addCustomProperty("Angles", [&]() {
        ImGui::DragFloat("Start Angle", &comp.angleStart, 1.0f, 0.0f, 360.0f);
        ImGui::DragFloat("End Angle", &comp.angleEnd, 1.0f, 0.0f, 360.0f);
    });
    
    inspector.addProperty("Resolution", &comp.resolution, 3, 128);
    
    inspector.addProperty("Filled", &comp.filled);
    if (comp.filled) {
        inspector.addProperty("Fill Color", &comp.fillColor);
    }
    
    inspector.addProperty("Stroked", &comp.stroked);
    if (comp.stroked) {
        inspector.addProperty("Stroke Color", &comp.color);
        inspector.addProperty("Line Width", &comp.lineWidth, 0.1f, 50.0f);
    }
    
    inspector.addCustomProperty("Info", [&]() {
        ImGui::Text("Angle Span: %.1f deg", comp.getAngleSpan());
        ImGui::Text("Arc Length: %.2f", comp.getArcLength());
    });
}

// ============================================================================
// Bezier Curve Component Inspector
// ============================================================================

void registerProperties(ecs::bezier_curve_component& comp, ComponentInspector& inspector) {
    inspector.addCustomProperty("Control Points", [&]() {
        ImGui::DragFloat2("P0 (Start)", &comp.p0.x, 1.0f);
        ImGui::DragFloat2("P1 (Control 1)", &comp.p1.x, 1.0f);
        ImGui::DragFloat2("P2 (Control 2)", &comp.p2.x, 1.0f);
        ImGui::DragFloat2("P3 (End)", &comp.p3.x, 1.0f);
    });
    
    inspector.addProperty("Resolution", &comp.resolution, 5, 200);
    inspector.addProperty("Color", &comp.color);
    inspector.addProperty("Line Width", &comp.lineWidth, 0.1f, 50.0f);
    
    inspector.addProperty("Show Control Points", &comp.showControlPoints);
    if (comp.showControlPoints) {
        inspector.addProperty("Control Point Color", &comp.controlPointColor);
        inspector.addProperty("Control Point Size", &comp.controlPointSize, 1.0f, 20.0f);
    }
    
    inspector.addCustomProperty("Info", [&]() {
        ImGui::Text("Approx Length: %.2f", comp.getApproximateLength());
    });
}

// ============================================================================
// Spline Component Inspector
// ============================================================================

void registerProperties(ecs::spline_component& comp, ComponentInspector& inspector) {
    inspector.addProperty("Tension", &comp.tension, 0.0f, 1.0f);
    inspector.addProperty("Resolution", &comp.resolution, 1, 50);
    inspector.addProperty("Closed", &comp.closed);
    
    inspector.addProperty("Color", &comp.color);
    inspector.addProperty("Line Width", &comp.lineWidth, 0.1f, 50.0f);
    
    inspector.addProperty("Show Control Points", &comp.showControlPoints);
    if (comp.showControlPoints) {
        inspector.addProperty("Control Point Color", &comp.controlPointColor);
        inspector.addProperty("Control Point Size", &comp.controlPointSize, 1.0f, 20.0f);
    }
    
    inspector.addCustomProperty("Stats", [&]() {
        ImGui::Text("Control Points: %zu", comp.size());
        ImGui::Text("Approx Length: %.2f", comp.getApproximateLength());
    });
    
    inspector.addCustomProperty("Operations", [&]() {
        if (ImGui::Button("Clear")) {
            comp.clear();
        }
    });
}

// ============================================================================
// Sprite Component Inspector
// ============================================================================

void registerProperties(ecs::sprite_component& comp, ComponentInspector& inspector) {
    inspector.addCustomProperty("Size", [&]() {
        ImGui::DragFloat("Width", &comp.width, 1.0f, 1.0f, 10000.0f);
        ImGui::DragFloat("Height", &comp.height, 1.0f, 1.0f, 10000.0f);
    });
    
    inspector.addCustomProperty("Anchor", [&]() {
        ImGui::DragFloat2("Anchor (0-1)", &comp.anchor.x, 0.01f, 0.0f, 1.0f);
        
        if (ImGui::Button("TL")) { comp.anchor = glm::vec2(0, 0); }
        ImGui::SameLine();
        if (ImGui::Button("TC")) { comp.anchor = glm::vec2(0.5f, 0); }
        ImGui::SameLine();
        if (ImGui::Button("TR")) { comp.anchor = glm::vec2(1, 0); }
        
        if (ImGui::Button("ML")) { comp.anchor = glm::vec2(0, 0.5f); }
        ImGui::SameLine();
        if (ImGui::Button("MC")) { comp.anchor = glm::vec2(0.5f, 0.5f); }
        ImGui::SameLine();
        if (ImGui::Button("MR")) { comp.anchor = glm::vec2(1, 0.5f); }
        
        if (ImGui::Button("BL")) { comp.anchor = glm::vec2(0, 1); }
        ImGui::SameLine();
        if (ImGui::Button("BC")) { comp.anchor = glm::vec2(0.5f, 1); }
        ImGui::SameLine();
        if (ImGui::Button("BR")) { comp.anchor = glm::vec2(1, 1); }
    });
    
    inspector.addProperty("Tint", &comp.tint);
    inspector.addProperty("Alpha", &comp.alpha, 0.0f, 1.0f);
    
    inspector.addCustomProperty("Flip", [&]() {
        ImGui::Checkbox("Flip X", &comp.flipX);
        ImGui::SameLine();
        ImGui::Checkbox("Flip Y", &comp.flipY);
    });
    
    inspector.addProperty("Use Source Rect", &comp.useSourceRect);
    if (comp.useSourceRect) {
        inspector.addCustomProperty("Source Rect", [&]() {
            ImGui::DragFloat4("XYWH", &comp.srcRect.x, 1.0f);
        });
    }
    
    inspector.addCustomProperty("Texture", [&]() {
        if (comp.texture.isAllocated()) {
            ImGui::Text("Texture: %dx%d", (int)comp.texture.getWidth(), (int)comp.texture.getHeight());
        } else {
            ImGui::TextColored(ImVec4(1, 0.5f, 0, 1), "No texture loaded");
        }
    });
}

// ============================================================================
// Text 2D Component Inspector
// ============================================================================

void registerProperties(ecs::text_2d_component& comp, ComponentInspector& inspector) {
    inspector.addCustomProperty("Text", [&]() {
        static char textBuffer[1024];
        strncpy(textBuffer, comp.text.c_str(), sizeof(textBuffer) - 1);
        if (ImGui::InputTextMultiline("##text", textBuffer, sizeof(textBuffer), 
                                       ImVec2(-1, 60))) {
            comp.text = textBuffer;
        }
    });
    
    inspector.addProperty("Font Size", &comp.fontSize, 8, 200);
    inspector.addProperty("Color", &comp.color);
    
    inspector.addCustomProperty("Alignment", [&]() {
        int alignH = (int)comp.alignH;
        if (ImGui::Combo("Horizontal", &alignH, "Left\0Center\0Right\0")) {
            comp.alignH = (ecs::text_2d_component::HorizontalAlign)alignH;
        }
        
        int alignV = (int)comp.alignV;
        if (ImGui::Combo("Vertical", &alignV, "Top\0Middle\0Bottom\0")) {
            comp.alignV = (ecs::text_2d_component::VerticalAlign)alignV;
        }
    });
    
    inspector.addProperty("Line Height", &comp.lineHeight, 0.5f, 3.0f);
    inspector.addProperty("Letter Spacing", &comp.letterSpacing, -0.5f, 2.0f);
    
    inspector.addProperty("Word Wrap", &comp.wordWrap);
    if (comp.wordWrap) {
        inspector.addProperty("Wrap Width", &comp.wrapWidth, 50.0f, 2000.0f);
    }
    
    inspector.addCustomProperty("Font", [&]() {
        if (comp.fontLoaded) {
            ImGui::TextColored(ImVec4(0, 1, 0, 1), "Font loaded");
        } else {
            ImGui::TextColored(ImVec4(1, 0.5f, 0, 1), "Using bitmap font");
        }
        
        ImGui::Text("Size: %.1f x %.1f", comp.getWidth(), comp.getHeight());
    });
}

// ============================================================================
// Grid Component Inspector
// ============================================================================

void registerProperties(ecs::grid_component& comp, ComponentInspector& inspector) {
    inspector.addProperty("Width", &comp.width, 1.0f, 10000.0f);
    inspector.addProperty("Height", &comp.height, 1.0f, 10000.0f);
    inspector.addProperty("Offset X", &comp.offsetX);
    inspector.addProperty("Offset Y", &comp.offsetY);
    inspector.addProperty("Major Spacing", &comp.majorSpacing, 1.0f, 1000.0f);
    inspector.addProperty("Minor Spacing", &comp.minorSpacing, 1.0f, 500.0f);
    inspector.addProperty("Major Line Color", &comp.majorLineColor);
    inspector.addProperty("Minor Line Color", &comp.minorLineColor);
    inspector.addProperty("Draw Major Lines", &comp.drawMajorLines);
    inspector.addProperty("Draw Minor Lines", &comp.drawMinorLines);
    inspector.addProperty("Draw Axes", &comp.drawAxes);
}

// ============================================================================
// Progress Bar Component Inspector
// ============================================================================

void registerProperties(ecs::progress_bar_component& comp, ComponentInspector& inspector) {
    inspector.addProperty("Value", &comp.value, 0.0f, 1.0f);
    inspector.addProperty("X", &comp.x);
    inspector.addProperty("Y", &comp.y);
    inspector.addProperty("Width", &comp.width, 1.0f, 1000.0f);
    inspector.addProperty("Height", &comp.height, 1.0f, 200.0f);
    inspector.addProperty("Corner Radius", &comp.cornerRadius, 0.0f, 50.0f);
    inspector.addProperty("Fill Color", &comp.fillColor);
    inspector.addProperty("Background Color", &comp.backgroundColor);
    inspector.addProperty("Border Color", &comp.borderColor);
    inspector.addProperty("Text Color", &comp.textColor);
    inspector.addProperty("Show Background", &comp.showBackground);
    inspector.addProperty("Show Border", &comp.showBorder);
    inspector.addProperty("Show Text", &comp.showText);
}

// ============================================================================
// Arrow Component Inspector
// ============================================================================

void registerProperties(ecs::arrow_component& comp, ComponentInspector& inspector) {
    inspector.addCustomProperty("Start", [&]() {
        ImGui::DragFloat2("Start", &comp.start.x, 1.0f);
    });
    inspector.addCustomProperty("End", [&]() {
        ImGui::DragFloat2("End", &comp.end.x, 1.0f);
    });
    inspector.addProperty("Color", &comp.color);
    inspector.addProperty("Line Width", &comp.lineWidth, 0.1f, 50.0f);
    inspector.addProperty("Head Length", &comp.headLength, 1.0f, 100.0f);
    inspector.addProperty("Head Angle", &comp.headAngle, 0.1f, 1.5f);
    inspector.addProperty("Double Ended", &comp.doubleEnded);
}

// ============================================================================
// Star Component Inspector
// ============================================================================

void registerProperties(ecs::star_component& comp, ComponentInspector& inspector) {
    inspector.addProperty("X", &comp.x);
    inspector.addProperty("Y", &comp.y);
    inspector.addProperty("Points", &comp.numPoints, 3, 20);
    inspector.addProperty("Inner Radius", &comp.innerRadius, 1.0f, 500.0f);
    inspector.addProperty("Outer Radius", &comp.outerRadius, 1.0f, 500.0f);
    inspector.addProperty("Filled", &comp.filled);
    if (comp.filled) inspector.addProperty("Fill Color", &comp.fillColor);
    inspector.addProperty("Stroked", &comp.stroked);
    if (comp.stroked) {
        inspector.addProperty("Stroke Color", &comp.strokeColor);
        inspector.addProperty("Stroke Width", &comp.strokeWidth, 0.1f, 50.0f);
    }
}

// ============================================================================
// Regular Polygon Component Inspector
// ============================================================================

void registerProperties(ecs::regular_polygon_component& comp, ComponentInspector& inspector) {
    inspector.addProperty("X", &comp.x);
    inspector.addProperty("Y", &comp.y);
    inspector.addProperty("Sides", &comp.numSides, 3, 20);
    inspector.addProperty("Radius", &comp.radius, 1.0f, 500.0f);
    inspector.addProperty("Rotation", &comp.rotation, 0.0f, 360.0f);
    inspector.addProperty("Filled", &comp.filled);
    if (comp.filled) inspector.addProperty("Fill Color", &comp.fillColor);
    inspector.addProperty("Stroked", &comp.stroked);
    if (comp.stroked) {
        inspector.addProperty("Stroke Color", &comp.strokeColor);
        inspector.addProperty("Stroke Width", &comp.strokeWidth, 0.1f, 50.0f);
    }
}

// ============================================================================
// Ring Component Inspector
// ============================================================================

void registerProperties(ecs::ring_component& comp, ComponentInspector& inspector) {
    inspector.addProperty("X", &comp.x);
    inspector.addProperty("Y", &comp.y);
    inspector.addProperty("Inner Radius", &comp.innerRadius, 1.0f, 500.0f);
    inspector.addProperty("Outer Radius", &comp.outerRadius, 1.0f, 500.0f);
    inspector.addProperty("Resolution", &comp.resolution, 8, 128);
    inspector.addProperty("Filled", &comp.filled);
    if (comp.filled) inspector.addProperty("Fill Color", &comp.fillColor);
    inspector.addProperty("Stroked", &comp.stroked);
    if (comp.stroked) {
        inspector.addProperty("Stroke Color", &comp.strokeColor);
        inspector.addProperty("Stroke Width", &comp.strokeWidth, 0.1f, 50.0f);
    }
}

// ============================================================================
// Cross Component Inspector
// ============================================================================

void registerProperties(ecs::cross_component& comp, ComponentInspector& inspector) {
    inspector.addProperty("X", &comp.x);
    inspector.addProperty("Y", &comp.y);
    inspector.addProperty("Width", &comp.width, 1.0f, 500.0f);
    inspector.addProperty("Height", &comp.height, 1.0f, 500.0f);
    inspector.addProperty("Thickness", &comp.thickness, 1.0f, 200.0f);
    inspector.addProperty("Filled", &comp.filled);
    if (comp.filled) inspector.addProperty("Fill Color", &comp.fillColor);
    inspector.addProperty("Stroked", &comp.stroked);
    if (comp.stroked) {
        inspector.addProperty("Stroke Color", &comp.strokeColor);
        inspector.addProperty("Stroke Width", &comp.strokeWidth, 0.1f, 50.0f);
    }
}

// ============================================================================
// Heart Component Inspector
// ============================================================================

void registerProperties(ecs::heart_component& comp, ComponentInspector& inspector) {
    inspector.addProperty("X", &comp.x);
    inspector.addProperty("Y", &comp.y);
    inspector.addProperty("Size", &comp.size, 1.0f, 500.0f);
    inspector.addProperty("Resolution", &comp.resolution, 8, 128);
    inspector.addProperty("Filled", &comp.filled);
    if (comp.filled) inspector.addProperty("Fill Color", &comp.fillColor);
    inspector.addProperty("Stroked", &comp.stroked);
    if (comp.stroked) {
        inspector.addProperty("Stroke Color", &comp.strokeColor);
        inspector.addProperty("Stroke Width", &comp.strokeWidth, 0.1f, 50.0f);
    }
}

// ============================================================================
// Sacred Geometry Component Inspectors
// ============================================================================

void registerProperties(ecs::vesica_piscis_component& comp, ComponentInspector& inspector) {
    inspector.addCustomProperty("Position", [&]() {
        ImGui::DragFloat("X", &comp.x, 1.0f);
        ImGui::DragFloat("Y", &comp.y, 1.0f);
    });
    
    inspector.addCustomProperty("Size", [&]() {
        ImGui::DragFloat("Width", &comp.width, 1.0f, 1.0f, 10000.0f);
        ImGui::DragFloat("Height", &comp.height, 1.0f, 1.0f, 10000.0f);
    });
    
    inspector.addProperty("Resolution", &comp.resolution, 8, 128);
    
    inspector.addProperty("Filled", &comp.filled);
    if (comp.filled) {
        inspector.addProperty("Fill Color", &comp.fillColor);
    }
    
    inspector.addProperty("Stroked", &comp.stroked);
    if (comp.stroked) {
        inspector.addProperty("Stroke Color", &comp.strokeColor);
        inspector.addProperty("Stroke Width", &comp.strokeWidth, 0.1f, 50.0f);
    }
    
    inspector.addProperty("Show Construction", &comp.showConstruction);
    
    inspector.addCustomProperty("Info", [&]() {
        auto center = comp.getCenter();
        ImGui::Text("Center: (%.1f, %.1f)", center.x, center.y);
        
        glm::vec2 c1, c2;
        float radius;
        comp.getCircleCenters(c1, c2, radius);
        ImGui::Text("Circle Radius: %.1f", radius);
    });
}

void registerProperties(ecs::flower_of_life_component& comp, ComponentInspector& inspector) {
    inspector.addProperty("X", &comp.x);
    inspector.addProperty("Y", &comp.y);
    inspector.addProperty("Radius", &comp.radius, 1.0f, 500.0f);
    inspector.addProperty("Rings", &comp.rings, 1, 10);
    inspector.addProperty("Resolution", &comp.resolution, 8, 128);
    inspector.addProperty("Color", &comp.color);
    inspector.addProperty("Line Width", &comp.lineWidth, 0.1f, 10.0f);
    inspector.addProperty("Show Outer Circle", &comp.showOuterCircle);
}

void registerProperties(ecs::metatrons_cube_component& comp, ComponentInspector& inspector) {
    inspector.addProperty("X", &comp.x);
    inspector.addProperty("Y", &comp.y);
    inspector.addProperty("Inner Radius", &comp.innerRadius, 1.0f, 500.0f);
    inspector.addProperty("Outer Radius", &comp.outerRadius, 1.0f, 500.0f);
    inspector.addProperty("Color", &comp.color);
    inspector.addProperty("Line Width", &comp.lineWidth, 0.1f, 10.0f);
    inspector.addProperty("Show Circles", &comp.showCircles);
}

// ============================================================================
// Soft Mask Component Inspector
// ============================================================================

void registerProperties(ecs::soft_mask_component& comp, ComponentInspector& inspector) {
    inspector.addProperty("Inverted", &comp.inverted);
    inspector.addProperty("Softness", &comp.softness, 0.0f, 10.0f);
}

} // namespace inspector
