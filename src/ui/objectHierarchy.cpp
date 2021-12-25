#include "objectHierarchy.h"
#include <scene/scene.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

void renderObjectNode(std::shared_ptr<Object> o, Scene* s)
{
    if (s->inspectedObject == o)
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0, 0, 1));
    bool expanded = ImGui::TreeNode(o->getName().c_str());
    if (s->inspectedObject == o)
        ImGui::PopStyleColor();

    ImGui::SameLine();
    if (ImGui::SmallButton(std::string("Inspect##" + o->getName()).c_str()))
        s->inspectedObject = o;
    
    if (expanded) {
        for (auto child : o->children)
            renderObjectNode(child, s);
        ImGui::TreePop();
    }
}

void ObjectHierarchy::render() {
    ImGui::Begin("Object Hierarchy");
    for (auto obj : scene->objects)
        renderObjectNode(obj, scene.get());
    ImGui::SetWindowSize(ImVec2(0, 0)); // size to content
    ImGui::End();
}