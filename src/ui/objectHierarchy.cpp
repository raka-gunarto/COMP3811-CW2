#include "objectHierarchy.h"
#include <scene/scene.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

void renderObjectNode(std::shared_ptr<Object> o, Scene* s)
{
    ImGui::PushID(o.get());
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
    ImGui::SameLine();
    if (ImGui::SmallButton(std::string("Clone##" + o->getName()).c_str()))
        s->objects.push_back(o->clone());

    ImGui::PopID();
    if (expanded) {
        for (auto child : o->children)
            renderObjectNode(child, s);
        ImGui::TreePop();
    }
}

void ObjectHierarchy::render() {
    ImGui::Begin("Object Hierarchy");
    for (auto obj : scene->objects)
        if (obj)
            renderObjectNode(obj, scene.get());

    if (ImGui::Button("Create Empty Object###createEmptyObject"))
        scene->objects.push_back(std::shared_ptr<Object>(new Object(scene)));
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