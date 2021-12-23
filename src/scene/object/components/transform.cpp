#include "transform.h"

#include <glm/gtc/matrix_transform.hpp>

#include <scene/object/object.h>

#include <iostream>

glm::mat4 Transform::modelMatrix() {
    glm::mat4 model =
        glm::translate(glm::mat4(1.0f), position) *
        glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1.0f, 0, 0)) *
        glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), glm::vec3(0, 1.0f, 0)) *
        glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), glm::vec3(0, 0, 1.0f)) *
        glm::scale(glm::mat4(1.0f), scale);

    if (object->getParent() == nullptr) // parentless
        return model;

    // find parent transform
    std::shared_ptr<Transform> t = nullptr;
    for (auto component : object->getParent()->components)
        if (component->getName() == "Transform")
        {
            t = std::dynamic_pointer_cast<Transform>(component);
            break;
        }
    if (t == nullptr)
    {
        std::cout << "WARN::" << object->getName() << "::TRANSFORM::cannot find parent transform (" << object->getParent()->getName() << ")" << std::endl;
        return model;
    }

    // apply transformations to parent mat first
    model = model * t->modelMatrix();
    return model;
}

void Transform::renderInspector() {

}