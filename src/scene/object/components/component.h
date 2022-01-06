#pragma once

#include <string>
#include <memory>

class Object;

class Component
{
public:
    Component(std::shared_ptr<Object> obj) : object(obj) {}
    std::string getName() { return name; }

    // DANGEROUS! for use by Object::clone() ONLY
    void setObject(std::shared_ptr<Object> newObj) { object = newObj; }

    virtual void renderInspector() = 0;
    virtual std::shared_ptr<Component> clone(std::shared_ptr<Object> newObj) = 0;
    static void renderComponentChildWindow(std::shared_ptr<Object> obj);

    void remove();

protected:
    std::string name;
    std::shared_ptr<Object> object;
};