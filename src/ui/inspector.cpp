#include "inspector.h"
#include <scene/scene.h>
#include <scene/object/object.h>
#include <scene/object/components/transform.h>

#include <imgui.h>
#include <imgui_internal.h>
#include <ImGuizmo.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

void Inspector::render() {
    if (scene->inspectedObject == nullptr) return;
    ImGui::Begin(std::string("Inspector: " + scene->inspectedObject->getName() + "###inspectorWindow").c_str());

    char name[128];
    strcpy(name, scene->inspectedObject->getName().c_str());
    if (ImGui::InputText("Object Name", name, 128))
        scene->inspectedObject->setName(std::string(name));

    std::shared_ptr<Transform> transform = nullptr;
    for (auto component : scene->inspectedObject->components)
        if (component->getName() == "Transform")
        {
            transform = std::dynamic_pointer_cast<Transform>(component);
            break;
        }
    if (transform != nullptr)
    {
        if (!currentGizmoOperation) currentGizmoOperation = ImGuizmo::TRANSLATE;
        if (ImGui::RadioButton("Translate", currentGizmoOperation == ImGuizmo::TRANSLATE))
            currentGizmoOperation = ImGuizmo::TRANSLATE;
        ImGui::SameLine();
        if (ImGui::RadioButton("Rotate", currentGizmoOperation == ImGuizmo::ROTATE))
            currentGizmoOperation = ImGuizmo::ROTATE;
        ImGui::SameLine();
        if (ImGui::RadioButton("Scale", currentGizmoOperation == ImGuizmo::SCALE))
            currentGizmoOperation = ImGuizmo::SCALE;

        ImGuiIO& io = ImGui::GetIO();
        ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);

        glm::mat4 modmat = transform->localMatrix();
        glm::mat4 view = scene->activeCamera->getView();
        if (scene->inspectedObject->getParent() != nullptr) // special case, modify view matrix so we can directly edit local matrix
            for (auto component : scene->inspectedObject->getParent()->components)
                if (component->getName() == "Transform")
                {
                    view = view * std::dynamic_pointer_cast<Transform>(component)->modelMatrix();
                    break;
                }
        ImGuizmo::Manipulate(
            glm::value_ptr(view),
            glm::value_ptr(scene->activeCamera->getPerspective()),
            (ImGuizmo::OPERATION)currentGizmoOperation,
            ImGuizmo::MODE::LOCAL,
            glm::value_ptr(modmat)
        );

        glm::vec3 t;
        glm::quat r;
        glm::vec3 s;
        glm::vec3 skew;
        glm::vec4 pers;
        glm::decompose(modmat, s, r, t, skew, pers);
        transform->position = t;
        transform->rotation = glm::degrees(glm::eulerAngles(r));
        transform->scale = s;
    }

    ImGui::Separator();
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