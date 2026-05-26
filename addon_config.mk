meta:
	ADDON_NAME = ofxEnTTInspector
	ADDON_DESCRIPTION = ImGui inspector UI for ofxEnTTKit ECS components. No app framework dependency.
	ADDON_AUTHOR = ofRasp
	ADDON_TAGS = "ecs" "entt" "imgui" "inspector"
	ADDON_URL = https://github.com/ofrasp/ofxEnTTInspector

common:
	ADDON_DEPENDENCIES = ofxEnTTKit ofxImGui ofxImGuiTextEdit ofxImGuiFileDialog ofxMagicEnum ofxAssimpModelLoader
	# src — #include "ComponentInspector.h", #include "inspectors/trace_inspectors.h"
	ADDON_INCLUDES += src

linux64:
linuxarmv6l:
linuxarmv7l:
linuxaarch64:
vs:
osx:
ios:
