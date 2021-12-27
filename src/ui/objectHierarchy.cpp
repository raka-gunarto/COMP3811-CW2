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

    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
    {
        ImGui::SetDragDropPayload("HIERARCHY_OBJECT", &o, sizeof(std::shared_ptr<Object>*)); // this is a little dirty i know
        ImGui::Text(o->getName().c_str());
        ImGui::EndDragDropSource();
    }

    if (ImGui::BeginDragDropTarget())
        if (const ImGuiPayload* p = ImGui::AcceptDragDropPayload("HIERARCHY_OBJECT"))
        {
            std::shared_ptr<Object> t = *(std::shared_ptr<Object>*)p->Data;
            t->reparent(o);
        }

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

    ImGui::Separator();
    ImGui::Text("Drop Here To Unparent");
    if (ImGui::BeginDragDropTarget())
        if (const ImGuiPayload* p = ImGui::AcceptDragDropPayload("HIERARCHY_OBJECT"))
        {
            std::shared_ptr<Object> t = *(std::shared_ptr<Object>*)p->Data;
            t->reparent(nullptr);
        }
    ImGui::SetWindowSize(ImVec2(0, 0)); // size to content
    ImGui::End();
}