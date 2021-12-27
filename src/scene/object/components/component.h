#pragma once

#include <string>
#include <memory>

class Object;

class Component
{
public:
    Component(std::shared_ptr<Object> obj) : object(obj) {}
    std::string getName() { return name; }
    virtual void renderInspector() = 0; 
    static void renderComponentChildWindow(std::shared_ptr<Object> obj);

protected:
    std::string name;
    std::shared_ptr<Object> object;
};