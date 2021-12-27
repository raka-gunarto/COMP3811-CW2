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
    void start() override;
    void update(std::shared_ptr<Scene> s) override;
    void renderInspector() override;

    float zoomSpeed = 0.5f;
    float rotateSpeed = 100.0f;
    float dragSpeed = 20.0f;
    float first = true;
private:
    std::shared_ptr<Transform> t;
};