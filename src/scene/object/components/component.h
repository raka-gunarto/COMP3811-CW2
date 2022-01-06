#pragma once

#include <string>
#include <memory>

#include <yaml-cpp/yaml.h>

class Object;

class Component
{
public:
    Component(std::shared_ptr<Object> obj) : object(obj) {}
    std::string getName() { return name; }

    // DANGEROUS! for use by Object::clone() ONLY
    void setObject(std::shared_ptr<Object> newObj) { object = newObj; }

    friend YAML::Emitter& operator << (YAML::Emitter& emitter, Component& c) {
        return c.serialise(emitter);
    }
    virtual YAML::Emitter& serialise(YAML::Emitter& emitter) {
        return emitter;
    };

    virtual void renderInspector() = 0;
    virtual std::shared_ptr<Component> clone(std::shared_ptr<Object> newObj) = 0;
    static void renderComponentChildWindow(std::shared_ptr<Object> obj);

    std::shared_ptr<Object> getObject() { return object; }

    void remove();

protected:
    std::string name;
    std::shared_ptr<Object> object;
};