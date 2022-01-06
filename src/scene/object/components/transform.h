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
    Transform(std::shared_ptr<Object> obj, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
        : Component(obj),
        position(position),
        rotation(rotation),
        scale(scale)
    {
        name = "Transform";
    }
    Transform(const Transform& other) = delete;
    Transform(const Transform& other, std::shared_ptr<Object> newObj) : Transform(newObj, other.position, other.rotation, other.scale) {}
    std::shared_ptr<Component> clone(std::shared_ptr<Object> newObj) override {
        return std::shared_ptr<Component>(new Transform(*this, newObj));
    }
    void renderInspector() override;

    YAML::Emitter& serialise(YAML::Emitter& emitter) override
    {
        emitter << YAML::BeginMap;
        emitter << YAML::Key << "name";
        emitter << YAML::Value << "Transform";
        emitter << YAML::Key << "position" << YAML::Value << YAML::Flow << YAML::BeginSeq
            << position.x << position.y << position.z
            << YAML::EndSeq;
        emitter << YAML::Key << "rotation" << YAML::Value << YAML::Flow << YAML::BeginSeq
            << rotation.x << rotation.y << rotation.z
            << YAML::EndSeq;
        emitter << YAML::Key << "scale" << YAML::Value << YAML::Flow << YAML::BeginSeq
            << scale.x << scale.y << scale.z
            << YAML::EndSeq;
        emitter << YAML::EndMap;
        return emitter;
    }

    glm::vec3 worldPos();
    glm::mat4 localMatrix();
    glm::mat4 modelMatrix();

    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
};