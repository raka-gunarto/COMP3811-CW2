#pragma once

#include <scene/object/components/component.h>
#include <scene/object/object.h>
#include <scene/scene.h>
#include <util/shader.h>

#include <memory>

class Object;
class Scene;
class Shader;

class Renderer : public Component
{
public:
    Renderer(std::shared_ptr<Object> obj) : Component(obj) {}
    virtual void render(std::shared_ptr<Scene> s) = 0;
    std::shared_ptr<Shader> shader;
};