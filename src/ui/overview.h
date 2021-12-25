#pragma once

#include <ui/window.h>

class Overview : public Window {
public:
    Overview(std::shared_ptr<Scene> s) : Window(s) {}
    void render() override;
};