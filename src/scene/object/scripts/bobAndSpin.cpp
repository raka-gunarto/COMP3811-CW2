#include "bobAndSpin.h"

#include <GLFW/glfw3.h>
#include <scene/scene.h>
#include <scene/object/components/transform.h>
#include <scene/object/components/light.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/quaternion.hpp>

#include <imgui.h>

#include <string>
#include <iostream>

void BobAndSpin::start() {
    // find and save transform
    for (auto component : object->components)
        if (component->getName() == "Transform")
        {
            t = std::dynamic_pointer_cast<Transform>(component);
            break;
        }
    if (t == nullptr)
        std::cout << "WARN::" << object->getName() << "::script::bobandspin::cannot find transform" << std::endl;
}

void BobAndSpin::update(std::shared_ptr<Scene> s)
{
    // do the bob
    t->position.y = 
        object->getParent()->getComponent<Transform>()->position.y +
        bobOffset +
        glm::sin(glfwGetTime() * bobSpeed) * bobSize;

    // do the spin ting
    t->rotation.y += s->dTime * spinSpeed;
}

void BobAndSpin::renderInspector()
{
    ImGui::Text(std::string("Script: " + name).c_str());
    ImGui::InputFloat("Bob Speed", &bobSpeed);
    ImGui::InputFloat("Bob Size", &bobSize);
    ImGui::InputFloat("Bob Offset", &bobOffset);
    ImGui::InputFloat("Spin Speed", &spinSpeed);
    ImGui::Separator();
}