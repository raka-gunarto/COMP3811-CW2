#pragma once

#include <scene/object/components/component.h>

#include <glm/glm.hpp>

#include <memory>

class Transform;
class Object;

class Camera : public Component {
public:
    Camera(std::shared_ptr<Object> obj) : Camera(obj, 0, 0, 45, 0.1, 10000) {}
    Camera(std::shared_ptr<Object> obj, int width, int height, float FOV, float near, float far);
    Camera(const Camera& other) = delete;
    Camera(const Camera& other, std::shared_ptr<Object> newObj) : Camera(newObj, other.width, other.height, other.FOV, other.near, other.far) {}
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
        emitter << YAML::EndMap;
        return emitter;
    }
    std::shared_ptr<Transform> transform;
    glm::mat4 getMatrix();
    glm::mat4 getPerspective();
    glm::mat4 getView();
private:
    int width, height;
    float FOV, near, far;
};