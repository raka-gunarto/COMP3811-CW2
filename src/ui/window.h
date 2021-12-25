#pragma once

#include <memory>

class Scene;
class Window {
public:
    Window(std::shared_ptr<Scene> s) : scene(s) {}
    virtual void render() = 0;

protected:
    std::shared_ptr<Scene> scene;
};