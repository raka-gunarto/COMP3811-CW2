#pragma once

#include <scene/object/components/component.h>

#include <glm/glm.hpp>

#include <memory>

class Transform;
class Object;

class Camera : public Component {
public:
    Camera(std::shared_ptr<Object> obj) : Camera(obj, 0, 0, 45, 0.1, 10000, 0.0f) {}
    Camera(std::shared_ptr<Object> obj, int width, int height, float FOV, float near, float far, float fogOffset);
    Camera(const Camera& other) = delete;
    Camera(const Camera& other, std::shared_ptr<Object> newObj) : Camera(newObj, other.width, other.height, other.FOV, other.near, other.far, other.fogOffset) {}
    std::shared_ptr<Component> clone(std::shared_ptr<Object> newObj) override {
        return std::shared_ptr<Component>(new Camera(*this, newObj));
    }
    void renderInspector() override;
    YAML::Emitter& serialise(YAML::Emitter& emitter) override
    {
        emitter << YAML::BeginMap;
        emitter << YAML::Key << "name";
        emitter << YAML::Value << "Camera";
        emitter << YAML::Key << "width" << YAML::Value << width;
        emitter << YAML::Key << "height" << YAML::Value << height;
        emitter << YAML::Key << "FOV" << YAML::Value << FOV;
        emitter << YAML::Key << "near" << YAML::Value << near;
        emitter << YAML::Key << "far" << YAML::Value << far;
        emitter << YAML::Key << "fogOffset" << YAML::Value << fogOffset;
        emitter << YAML::EndMap;
        return emitter;
    }
    void _deserialise(const YAML::Node& componentNode) override
    {
        // do the reverse of serialise duh
        width = componentNode["width"].as<float>(0.0f);
        height = componentNode["height"].as<float>(0.0f);
        FOV = componentNode["FOV"].as<float>(0.0f);
        near = componentNode["near"].as<float>(0.0f);
        far = componentNode["far"].as<float>(0.0f);
        fogOffset = componentNode["fogOffset"].as<float>(0.0f);
    }
    std::shared_ptr<Transform> transform;
    glm::mat4 getMatrix();
    glm::mat4 getPerspective();
    glm::mat4 getView();

    int width, height;
    float FOV, near, far, fogOffset;
};