#include "editCamera.h"

#include <GLFW/glfw3.h>
#include <scene/scene.h>
#include <scene/object/components/transform.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/quaternion.hpp>

#include <imgui.h>

#include <string>
#include <iostream>

void EditCamera::start() {
    // find and save transform
    for (auto component : object->components)
        if (component->getName() == "Transform")
        {
            t = std::dynamic_pointer_cast<Transform>(component);
            break;
        }
    if (t == nullptr)
        std::cout << "WARN::" << object->getName() << "::script::editcamera::cannot find transform" << std::endl;
}

void EditCamera::update(std::shared_ptr<Scene> s)
{
    // handle mouse scroll "zoom"
    // calculate direction vector
    glm::vec3 direction = glm::rotate(
        glm::quat(
            glm::vec3(
                glm::radians(t->rotation.x),
                glm::radians(t->rotation.y),
                glm::radians(t->rotation.z)
            )
        ),
        glm::vec3(0, 0, -1)
    );

    // multiple direction vector by speed and scroll delta
    glm::vec3 zoom = direction * zoomSpeed;
    zoom *= s->scrollY;

    // apply to position
    t->position += zoom;

    // rotate or drag camera 
    if (ImGui::GetIO().WantCaptureMouse) return;
    if (glfwGetMouseButton(s->window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        // get some window data
        int width, height;
        glfwGetWindowSize(s->window, &width, &height);

        double mouseX, mouseY;
        glfwGetCursorPos(s->window, &mouseX, &mouseY);

        if (first)
        {
            lastX = mouseX;
            lastY = mouseY;
            first = false;
            return;
        }
        float dX = (mouseX - lastX) / width;
        float dY = (mouseY - lastY) / height;

        // drag
        if (glfwGetKey(s->window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(s->window, GLFW_KEY_RIGHT_SHIFT == GLFW_PRESS))
        {
            t->position += -dX * dragSpeed * glm::normalize(glm::cross(direction, glm::vec3(0, 1.0f, 0)));
            t->position += dY * dragSpeed * glm::rotate(
                glm::quat(
                    glm::vec3(
                        glm::radians(t->rotation.x),
                        glm::radians(t->rotation.y),
                        glm::radians(t->rotation.z)
                    )
                ),
                glm::vec3(0, 1.0f, 0)
            );

        }
        else // rotate
        {
            direction = glm::rotate(direction, glm::radians(-dY * rotateSpeed), glm::normalize(glm::cross(direction, glm::vec3(0, 1.0f, 0))));
            direction = glm::rotate(direction, glm::radians(-dX * rotateSpeed), glm::vec3(0, 1.0f, 0));
            glm::vec3 o = glm::eulerAngles(glm::rotation(glm::vec3(0, 0, -1), glm::normalize(direction)));
            t->rotation.x = glm::degrees(o.x);
            t->rotation.y = glm::degrees(o.y);
            t->rotation.z = glm::degrees(o.z);
        }

        lastX = mouseX;
        lastY = mouseY;
    }
    else first = true;
}

void EditCamera::renderInspector()
{
    ImGui::Text(std::string("Script: " + name).c_str());
    ImGui::DragFloat("Zoom Speed", &zoomSpeed, 0.1f);
    ImGui::DragFloat("Rotate Speed", &rotateSpeed, 1.0f);
    ImGui::DragFloat("Drag Speed", &dragSpeed, 1.0f);
    ImGui::Separator();
}