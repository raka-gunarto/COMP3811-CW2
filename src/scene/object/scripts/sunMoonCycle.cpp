#include "sunMoonCycle.h"

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

void SunMoonCycle::start() {
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

void SunMoonCycle::update(std::shared_ptr<Scene> s)
{
    double deltaTime = s->dTime;
    deltaTime /= 60 * 60 * 24;
    deltaTime *= timeScale;
    time += deltaTime;
    time = fmod(time, 1.0);
    float angle;
    angle = ((time < 0.5 ? time / 0.5 : (time - 0.5) / 0.5) * 180) - 90;
    t->rotation.z = angle;

    auto light = object->children[0]->getComponent<Light>();
    if (time < 0.5)
    {
        // sun cycle
        light->color = glm::vec3(0.738245487f, 0.764705896f, 0.58477509f);
        s->backgroundColor = glm::mix(glm::vec3(0.85098039215f, 0.3725490196f, 0.18431372549f), glm::vec3(1.0f, 1.0f, 1.0f),
            time < 0.25 ? time / 0.25 : (0.5 - time) / 0.25);
    }
    else {
        // moon cycle
        light->color = glm::vec3(0.4431372549f, 0.56470588235f, 0.67058823529f);
        s->backgroundColor = glm::vec3(0.0f);
    }
}

void SunMoonCycle::renderInspector()
{
    ImGui::Text(std::string("Script: " + name).c_str());
    ImGui::Text("Time: %.1f hours", time * 24);
    ImGui::InputDouble("Time Scale", &timeScale);
    ImGui::Separator();
}