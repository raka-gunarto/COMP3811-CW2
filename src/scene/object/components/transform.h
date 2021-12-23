#pragma once

#include <glm/glm.hpp>

#include <scene/object/components/component.h>

#include <string>
#include <memory>

class Object;

class Transform : public Component
{
public:
    Transform(std::shared_ptr<Object> obj)
        : Component(obj),
        position(glm::vec3(0.0)),
        rotation(glm::vec3(0.0)),
        scale(glm::vec3(1.0))
    {
        name = "Transform";
    }
    void renderInspector() override;

    glm::mat4 modelMatrix();

    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
};