#include "scene.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

void Scene::update() {
    for (auto obj : objects)
        obj->update(this->shared_from_this());
}

void Scene::render() {
    for (auto obj : objects)
        obj->render(this->shared_from_this());
}

void Scene::renderUI() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    renderUI_Overview();
    renderUI_ObjectTree();
    renderUI_Inspector();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Scene::renderUI_Overview() {
    // render scene overview
    ImGui::Begin("Overview");

    ImGui::Text("Performance:");
    ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    ImGui::Separator();

    ImGui::Text("Settings:");
    if (ImGui::Checkbox("VSync", &vsync))  // has vsync changed
        glfwSwapInterval(vsync); // set vsync

    ImGui::SetWindowSize(ImVec2(0, 0)); // size to content
    ImGui::End();
}

void renderObjectNode(std::shared_ptr<Object> o, Scene* s)
{
    if (s->inspectedObject == o)
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0, 0, 0));
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

void Scene::renderUI_ObjectTree() {
    ImGui::Begin("Object Hierarchy");
    for (auto obj : objects)
        renderObjectNode(obj, this);
    ImGui::SetWindowSize(ImVec2(0, 0)); // size to content
    ImGui::End();
}

void Scene::renderUI_Inspector() {
    if (inspectedObject == nullptr) return;
    ImGui::Begin(std::string("Inspector: " + inspectedObject->getName()).c_str());
    for (auto component : inspectedObject->components)
        component->renderInspector();
    ImGui::End();
}