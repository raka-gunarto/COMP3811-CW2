#pragma once

#include <glm/glm.hpp>

#include <scene/object/components/component.h>
#include <scene/object/scripts/script.h>
#include <scene/scene.h>

#include <yaml-cpp/yaml.h>

#include <vector>
#include <memory>
#include <string>

class Scene;
class Component;
class Object;
class Script;

class Object : public std::enable_shared_from_this<Object>
{
public:
    Object(std::shared_ptr<Scene> s) : scene(s) {}
    Object(const Object& other) = delete; // use clone()

    std::vector<std::shared_ptr<Component>> components;
    std::vector<std::shared_ptr<Object>> children;
    std::vector<std::shared_ptr<Script>> scripts;

    void setName(std::string name) { this->name = name; }
    std::string getName() { return name; }
    std::shared_ptr<Object> getParent() { return parent; }
    std::shared_ptr<Scene> getScene() { return scene; }

    std::shared_ptr<Object> clone(std::shared_ptr<Object> parent = nullptr);

    template<typename C>
    std::shared_ptr<C> getComponent()
    {
        std::shared_ptr<C> c = nullptr;
        for (auto component : this->components)
        {
            c = std::dynamic_pointer_cast<C>(component);
            if (c != nullptr)
                return c;
        }
        return c;
    }
    void reparent(std::shared_ptr<Object> p, bool blueprint = false);
    void remove();
    void update(std::shared_ptr<Scene> s);
    void render(std::shared_ptr<Scene> s);

    friend YAML::Emitter& operator << (YAML::Emitter& emitter, const Object& o);
    static std::shared_ptr<Object> deserialise(const YAML::Node& objectNode, std::shared_ptr<Scene> s);

protected:
    std::shared_ptr<Scene> scene;
    std::string name;
    std::shared_ptr<Object> parent = nullptr;
};