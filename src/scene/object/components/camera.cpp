#include "camera.h"

#include <imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include <scene/object/object.h>
#include <scene/object/components/transform.h>

#include <iostream>

Camera::Camera(std::shared_ptr<Object> obj, int width, int height, float FOV, float near, float far)
    : Component(obj),
    width(width),
    height(height),
    FOV(FOV),
    near(near),
    far(far)
{
    name = "Camera";

    // find object transform
    std::shared_ptr<Transform> t = nullptr;
    for (auto component : obj->components)
        if (component->getName() == "Transform") {
            t = std::dynamic_pointer_cast<Transform>(component);
            break;
        }

    // cannot find transform
    if (t == nullptr)
    {
        std::cout << "ERROR::" << object->getName() << "::camera::cannot find transform" << std::endl;
        return;
    }

    transform = t;
}

glm::mat4 Camera::getMatrix() {
    // calculate direction vector
    glm::vec3 direction = glm::rotate(
        glm::quat(
            glm::vec3(
                glm::radians(transform->rotation.x),
                glm::radians(transform->rotation.y),
                glm::radians(transform->rotation.z)
            )
        ),
        glm::vec3(0, 0, -1)
    );

    return
        // calculate projection matrix
        glm::perspective(glm::radians(FOV), (float)width / height, near, far) *
        // calculate view matrix
        glm::lookAt(transform->position, transform->position + direction, glm::vec3(0, 1.0f, 0));
}

void Camera::renderInspector() {
    ImGui::Text("Camera");
    ImGui::DragInt("Width", &width);
    ImGui::DragInt("Height", &height);
    ImGui::DragFloat("FOV", &FOV, 0.1f);
    ImGui::DragFloat("Near Plane", &near, 0.1f);
    ImGui::DragFloat("Far Plane", &far, 0.1f);
    ImGui::Separator();
}