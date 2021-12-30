#pragma once

#include <scene/object/components/component.h>

#include <glm/glm.hpp>

#include <memory>

class Transform;
class Object;

class Camera : public Component {
public:
    Camera(std::shared_ptr<Object> obj) : Camera(obj, 0, 0, 0, 0, 0) {}
    Camera(std::shared_ptr<Object> obj, int width, int height, float FOV, float near, float far);
    Camera(const Camera& other) = delete;
    Camera(const Camera& other, std::shared_ptr<Object> newObj) : Camera(newObj, other.width, other.height, other.FOV, other.near, other.far) {}
    std::shared_ptr<Component> clone(std::shared_ptr<Object> newObj) override {
        return std::shared_ptr<Component>(new Camera(*this, newObj));
    }
    void renderInspector() override;
    std::shared_ptr<Transform> transform;
    glm::mat4 getMatrix();
    glm::mat4 getPerspective();
    glm::mat4 getView();
private:
    int width, height;
    float FOV, near, far;
};