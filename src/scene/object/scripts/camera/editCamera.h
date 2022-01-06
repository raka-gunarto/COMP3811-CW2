#pragma once

#include <scene/object/scripts/script.h>

#include <string>
#include <memory>

class Object;
class Scene;
class Transform;

class EditCamera : public Script {
public:
    EditCamera(std::shared_ptr<Object> obj) : Script("EditCamera", obj) {
        start();
    }
    EditCamera(std::shared_ptr<Object> obj, float zoomSpeed, float rotateSpeed, float dragSpeed)
        : EditCamera(obj)
    {
        this->zoomSpeed = zoomSpeed;
        this->rotateSpeed = rotateSpeed;
        this->dragSpeed = dragSpeed;
    }

    EditCamera(const EditCamera& other) = delete;
    EditCamera(const EditCamera& other, std::shared_ptr<Object> newObj)
        : EditCamera(newObj, other.zoomSpeed, other.rotateSpeed, other.dragSpeed) {}
    void start() override;
    void update(std::shared_ptr<Scene> s) override;
    void renderInspector() override;
    YAML::Emitter& serialise(YAML::Emitter& emitter) override 
    {
        emitter << YAML::BeginMap;
        emitter << YAML::Key << "name";
        emitter << YAML::Value << "EditCamera";
        emitter << YAML::Key << "zoomSpeed" << YAML::Value << zoomSpeed;
        emitter << YAML::Key << "rotateSpeed" << YAML::Value << rotateSpeed;
        emitter << YAML::Key << "dragSpeed" << YAML::Value << dragSpeed;
        emitter << YAML::EndMap;
        return emitter;
    }

    std::shared_ptr<Script> clone(std::shared_ptr<Object> newObj) {
        return std::shared_ptr<Script>(new EditCamera(*this, newObj));
    }

    float zoomSpeed = 0.5f;
    float rotateSpeed = 100.0f;
    float dragSpeed = 20.0f;
    float first = true;
    double lastX, lastY;
private:
    std::shared_ptr<Transform> t;
};