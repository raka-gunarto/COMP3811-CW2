#pragma once

#include <ui/window.h>

class Assets : public Window {
public:
    Assets(std::shared_ptr<Scene> s) : Window(s) {}
    void render() override;
};