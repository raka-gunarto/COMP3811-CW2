#pragma once

#include <scene/object/components/component.h>

class MeshRenderer : public Component {
public:
    MeshRenderer(std::shared_ptr<Object> obj)
        : Component(obj) {}

    void renderInspector() override {}
};