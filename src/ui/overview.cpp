#include "overview.h"
#include <scene/scene.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glm/gtc/type_ptr.hpp>

void Overview::render() {
    ImGui::Begin("Overview");

    ImGui::Text("Performance:");
    ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    ImGui::Separator();

    if (ImGui::Button("Save Scene"))
        scene->save();

    ImGui::Separator();

    ImGui::Text("Settings:");
    if (ImGui::Checkbox("VSync", &(scene->vsync)))  // has vsync changed
        glfwSwapInterval(scene->vsync); // set vsync

    ImGui::Separator();
    ImGui::ColorEdit3("Ambient Color", glm::value_ptr(scene->ambientColor));
    ImGui::SliderFloat("Ambient Intensity", &scene->ambientIntensity, 0.0f, 1.0f);
    ImGui::Text("Active Camera: ");
    ImGui::SameLine();
    ImGui::Button(scene->activeCamera ? scene->activeCamera->getObject()->getName().c_str() : "NONE");
    if (ImGui::BeginDragDropTarget())
        if (const ImGuiPayload* p = ImGui::AcceptDragDropPayload("HIERARCHY_OBJECT"))
        {
            Object* obj = (*((Object**)p->Data));
            auto camera = obj->getComponent<Camera>();
            if (camera)
                scene->activeCamera = camera;
        }
    ImGui::SetWindowSize(ImVec2(0, 0)); // size to content
    ImGui::End();
}