#include "transform.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <imgui.h>

#include <scene/object/object.h>

#include <iostream>

glm::vec3 Transform::worldPos()
{
    return modelMatrix() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
}

glm::mat4 Transform::localMatrix() {
    glm::vec3 rotationRad(glm::radians(rotation.x), glm::radians(rotation.y), glm::radians(rotation.z));
    glm::mat4 model =
        glm::translate(glm::mat4(1.0f), position) *
        glm::toMat4(glm::quat(rotationRad)) *
        glm::scale(glm::mat4(1.0f), scale);
    return model;
}

glm::mat4 Transform::modelMatrix() {
    glm::vec3 rotationRad(glm::radians(rotation.x), glm::radians(rotation.y), glm::radians(rotation.z));
    glm::mat4 model =
        glm::translate(glm::mat4(1.0f), position) *
        glm::toMat4(glm::quat(rotationRad)) *
        glm::scale(glm::mat4(1.0f), scale);

    if (object->getParent() == nullptr) // parentless
        return model;

    // find parent transform
    std::shared_ptr<Transform> t = nullptr;
    for (auto component : object->getParent()->components)
        if (component->getName() == "Transform")
        {
            t = std::dynamic_pointer_cast<Transform>(component);
            break;
        }
    if (t == nullptr)
    {
        std::cout << "WARN::" << object->getName() << "::TRANSFORM::cannot find parent transform (" << object->getParent()->getName() << ")" << std::endl;
        return model;
    }

    // apply transformations to parent mat first
    model = t->modelMatrix() * model;
    return model;
}

void Transform::renderInspector() {
    ImGui::Text("Transform");
    ImGui::DragFloat3("Position", glm::value_ptr(position), 0.01f);
    ImGui::DragFloat3("Rotation", glm::value_ptr(rotation));
    ImGui::DragFloat3("Scale", glm::value_ptr(scale), 0.01f, 0.01f);
    ImGui::NewLine();
    glm::vec3 worldpos = worldPos();
    ImGui::Text(std::string("World Coords: " + std::to_string(worldpos.x) + "," + std::to_string(worldpos.y) + "," + std::to_string(worldpos.z)).c_str());
    ImGui::Separator();
}