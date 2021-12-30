#pragma once

#include <scene/scene.h>
#include <scene/object/object.h>

#include <memory>
#include <string>

class Scene;
class Object;

class Script {
public:
    Script(std::string name, std::shared_ptr<Object> obj) : name(name), object(obj) {
    }
    virtual void start() = 0;
    virtual void update(std::shared_ptr<Scene> s) = 0;
    virtual void renderInspector() = 0;
    static void renderComponentChildWindow(std::shared_ptr<Object> obj);

    // DANGEROUS! for use by Object::clone() ONLY
    void setObject(std::shared_ptr<Object> newObj) { object = newObj; }
    virtual std::shared_ptr<Script> clone(std::shared_ptr<Object> newObj) = 0;
protected:
    std::string name;
    std::shared_ptr<Object> object;
};