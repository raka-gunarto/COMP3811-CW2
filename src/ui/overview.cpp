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

    ImGui::Text("Settings:");
    if (ImGui::Checkbox("VSync", &(scene->vsync)))  // has vsync changed
        glfwSwapInterval(scene->vsync); // set vsync

    ImGui::Separator();
    ImGui::ColorEdit3("Ambient Color", glm::value_ptr(scene->ambientColor));
    ImGui::SliderFloat("Ambient Intensity", &scene->ambientIntensity, 0.0f, 1.0f);

    ImGui::SetWindowSize(ImVec2(0, 0)); // size to content
    ImGui::End();
}