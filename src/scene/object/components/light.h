#pragma once

#include <scene/object/components/component.h>

#include <glm/glm.hpp>

#include <memory>

class Transform;

class Light : public Component
{
public:
    Light(std::shared_ptr<Object> obj, glm::vec3 color = glm::vec3(1.0f), float linearAttenuation = 0, float quadAttenuation = 0);
    Light(const Light& other, std::shared_ptr<Object> newObj) : Light(newObj, other.color, other.linearAttenuation, other.quadAttenuation) {}
    std::shared_ptr<Component> clone(std::shared_ptr<Object> newObj) override {
        return std::shared_ptr<Component> (new Light(*this, newObj));
    }
    void renderInspector() override;

    float linearAttenuation = 0;
    float quadAttenuation = 0;
    glm::vec3 color = glm::vec3(1.0f);

    std::shared_ptr<Transform> transform;
};