#pragma once

#include <ui/window.h>

class Inspector : public Window {
public:
    Inspector(std::shared_ptr<Scene> s) : Window(s) {}
    void render() override;

    int currentGizmoOperation = 0;
};