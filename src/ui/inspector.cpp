#include "inspector.h"
#include <scene/scene.h>

#include <imgui.h>

void Inspector::render() {
    if (scene->inspectedObject == nullptr) return;
    ImGui::Begin(std::string("Inspector: " + scene->inspectedObject->getName() + "###inspectorWindow").c_str());
    for (auto component : scene->inspectedObject->components)
        component->renderInspector();
    ImGui::End();
}