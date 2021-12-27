#include "inspector.h"
#include <scene/scene.h>

#include <imgui.h>

void Inspector::render() {
    if (scene->inspectedObject == nullptr) return;
    ImGui::Begin(std::string("Inspector: " + scene->inspectedObject->getName() + "###inspectorWindow").c_str());
    for (auto component : scene->inspectedObject->components)
    {
        ImGui::PushID(component.get());
        component->renderInspector();
        ImGui::PopID();
    }
    for (auto script : scene->inspectedObject->scripts)
    {
        ImGui::PushID(script.get());
        script->renderInspector();
        ImGui::PopID();
    }

    Component::renderComponentChildWindow(scene->inspectedObject);
    Script::renderComponentChildWindow(scene->inspectedObject);

    ImGui::End();
}