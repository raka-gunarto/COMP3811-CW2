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

    for (auto window : windowUIs)
        window->render();

    ImGui::Render();
    // saves and restores opengl state (i love you imgui <3)
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}