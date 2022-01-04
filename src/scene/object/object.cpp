#include "object.h"

#include <scene/object/components/renderer/renderer.h>
#include <scene/object/components/component.h>
#include <scene/object/scripts/script.h>


std::shared_ptr<Object> Object::clone(std::shared_ptr<Object> parent)
{
    std::shared_ptr<Object> newObj(new Object(this->scene));
    newObj->setName(name + " (Clone)");
    if (parent)
        newObj->parent = parent;

    // deep copy children
    for (auto child : children)
        newObj->children.push_back(child->clone(newObj));

    // deep copy components
    for (auto component : components)
        newObj->components.push_back(component->clone(newObj));

    // deep copy scripts
    for (auto script : scripts)
        newObj->scripts.push_back(script->clone(newObj));
    return newObj;
}

void Object::reparent(std::shared_ptr<Object> p)
{
    // do we have a parent? remove us from their children
    if (parent != nullptr)
        for (auto it = parent->children.begin(); it != parent->children.end(); ++it) {
            if (it->get() == this)
            {
                parent->children.erase(it);
                break;
            }
        }
    else // remove from scene direct child
        for (auto it = scene->objects.begin(); it != scene->objects.end(); ++it) {
            if (it->get() == this)
            {
                scene->objects.erase(it);
                break;
            }
        }

    // are we just setting to null?
    if (p == nullptr) {
        parent.reset();
        parent = nullptr;
        scene->objects.push_back(shared_from_this()); // add back to scene direct child
        return;
    }

    // find parent and add to children
    parent = p;
    parent->children.push_back(this->shared_from_this());
}

void Object::update(std::shared_ptr<Scene> s) {
    for (auto script : scripts)
        script->update(s);
    for (auto child : children)
        child->update(s);
}

void Object::render(std::shared_ptr<Scene> s) {
    for (auto component : components)
    {
        std::shared_ptr<Renderer> r = std::dynamic_pointer_cast<Renderer>(component);
        if (r != nullptr)
            r->render(s);
    }
    for (auto child : children)
        child->render(s);
}