#pragma once

#include <ui/window.h>

class ObjectHierarchy : public Window {
public:
    ObjectHierarchy(std::shared_ptr<Scene> s) : Window(s) {}
    void render() override;
};