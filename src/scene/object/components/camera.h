#pragma once

#include <scene/object/components/component.h>

#include <glm/glm.hpp>

#include <memory>

class Transform;
class Object;

class Camera : public Component {
public:
    Camera(std::shared_ptr<Object> obj, int width, int height, float FOV, float near, float far);
    void renderInspector() override;
    std::shared_ptr<Transform> transform;
    glm::mat4 getMatrix();
private:
    int width, height;
    float FOV, near, far;
};