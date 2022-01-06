#include "light.h"

#include <scene/object/object.h>
#include <scene/object/components/transform.h>

#include <imgui.h>

#include <glm/gtc/type_ptr.hpp>

#include <iostream>

Light::Light(std::shared_ptr<Object> obj, glm::vec3 color, float linearAttenuation, float quadAttenuation)
    : Component(obj),
    color(color),
    linearAttenuation(linearAttenuation),
    quadAttenuation(quadAttenuation)
{
    name = "Light";

    // find object transform
    std::shared_ptr<Transform> t = obj->getComponent<Transform>();

    // cannot find transform
    if (t == nullptr)
    {
        std::cout << "WARN::" << object->getName() << "::light::cannot find transform" << std::endl;
        return;
    }

    transform = t;
}

void Light::renderInspector()
{
    ImGui::Text("Light");
    if (ImGui::RadioButton("Rotate", type == POINT))
        type = POINT;
    ImGui::SameLine();
    if (ImGui::RadioButton("Scale", type == DIRECTIONAL))
        type = DIRECTIONAL;
    ImGui::ColorEdit3("Color", glm::value_ptr(color));
    if (type == POINT)
    {
        ImGui::DragFloat("Linear Attenuation", &linearAttenuation, 0.001f);
        ImGui::DragFloat("Quadratic Attenuation", &quadAttenuation, 0.001f);
    }
    ImGui::Separator();
}