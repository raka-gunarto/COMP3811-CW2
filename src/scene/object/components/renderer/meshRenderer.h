#pragma once

#include <scene/object/components/component.h>

#include <stdexcept>

class MeshRenderer : public Component {
public:
    MeshRenderer(std::shared_ptr<Object> obj)
        : Component(obj) {}

    void renderInspector() override {}
    std::shared_ptr<Component> clone(std::shared_ptr<Object> newObj) {
        throw std::runtime_error("not implemented yet");
        return std::shared_ptr<Component>(nullptr);
    }
};