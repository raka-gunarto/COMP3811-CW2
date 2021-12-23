#include "object.h"

#include <scene/object/components/renderer/renderer.h>

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

    // are we just setting to null?
    if (p == nullptr) {
        parent.reset();
        parent = nullptr;
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