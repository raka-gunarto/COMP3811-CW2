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
        Object* ptr = o.get();
        ImGui::SetDragDropPayload("HIERARCHY_OBJECT", &ptr, sizeof(Object*)); // this is a little dirty i know
        ImGui::Text(o->getName().c_str());
        ImGui::EndDragDropSource();
    }

    if (ImGui::BeginDragDropTarget())
        if (const ImGuiPayload* p = ImGui::AcceptDragDropPayload("HIERARCHY_OBJECT"))
        {
            (*((Object**)p->Data))->reparent(o);
        }

    ImGui::SameLine();
    if (ImGui::SmallButton(std::string("Inspect##" + o->getName()).c_str()))
        s->inspectedObject = o;
    ImGui::SameLine();
    if (ImGui::SmallButton(std::string("Clone##" + o->getName()).c_str()))
        s->objects.push_back(o->clone());
    ImGui::SameLine();
    if (ImGui::SmallButton(std::string("Delete##" + o->getName()).c_str()))
    {
        o->remove();
        ImGui::PopID();
        if (expanded) ImGui::TreePop();
        return;
    }

    ImGui::PopID();
    if (expanded) {
        for (auto child : o->children)
            if (child)
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
            (*((Object**)p->Data))->reparent(nullptr);
    ImGui::End();
}