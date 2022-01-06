#include "camera.h"

#include <imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>

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
    std::shared_ptr<Transform> t = obj->getComponent<Transform>();

    // cannot find transform
    if (t == nullptr)
    {
        std::cout << "WARN::" << object->getName() << "::camera::cannot find transform" << std::endl;
        return;
    }

    transform = t;

    // default width height
    if (width == 0 || height == 0)
    {
        const auto mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        width = mode->width;
        height = mode->height;
    }
}

glm::mat4 Camera::getMatrix() {
    // calculate direction vector
    // FIXME: there has to be a better way than having to call decompose
    glm::mat4 model = transform->modelMatrix();
    glm::vec3 pos, scale, skew;
    glm::vec4 pers;
    glm::quat rot;
    glm::decompose(model, scale, rot, pos, skew, pers);

    glm::vec3 direction = glm::rotate(
        rot,
        glm::vec3(0, 0, -1)
    );

    return
        // calculate projection matrix
        glm::perspective(glm::radians(FOV), (float)width / height, near, far) *
        // calculate view matrix
        glm::lookAt(pos, pos + direction, glm::vec3(0, 1.0f, 0));
}

glm::mat4 Camera::getPerspective() {
    return
        glm::perspective(glm::radians(FOV), (float)width / height, near, far);
}

glm::mat4 Camera::getView() {
    // calculate direction vector
    // FIXME: there has to be a better way than having to call decompose
    glm::mat4 model = transform->modelMatrix();
    glm::vec3 pos, scale, skew;
    glm::vec4 pers;
    glm::quat rot;
    glm::decompose(model, scale, rot, pos, skew, pers);

    glm::vec3 direction = glm::rotate(
        rot,
        glm::vec3(0, 0, -1)
    );

    return
        glm::lookAt(pos, pos + direction, glm::vec3(0, 1.0f, 0));
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