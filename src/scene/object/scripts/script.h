#pragma once

#include <scene/scene.h>
#include <scene/object/object.h>

#include <memory>
#include <string>

#include <yaml-cpp/yaml.h>

class Scene;
class Object;

class Script {
public:
    Script(std::string name, std::shared_ptr<Object> obj) : name(name), object(obj) {
    }
    virtual void start() = 0;
    virtual void update(std::shared_ptr<Scene> s) = 0;
    friend YAML::Emitter& operator << (YAML::Emitter& emitter, Script& c) {
        return c.serialise(emitter);
    }
    virtual YAML::Emitter& serialise(YAML::Emitter& emitter) {
        return emitter;
    };
    static std::shared_ptr<Script> deserialise(const YAML::Node& componentNode, std::shared_ptr<Object> obj);
    virtual void _deserialise(const YAML::Node& componentNode) = 0;

    virtual void renderInspector() = 0;
    static void renderComponentChildWindow(std::shared_ptr<Object> obj);

    // DANGEROUS! for use by Object::clone() ONLY
    void setObject(std::shared_ptr<Object> newObj) { object = newObj; }
    virtual std::shared_ptr<Script> clone(std::shared_ptr<Object> newObj) = 0;
    
    void remove();
protected:
    std::string name;
    std::shared_ptr<Object> object;
};