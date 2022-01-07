#include "object.h"

#include <scene/object/components/renderer/renderer.h>
#include <scene/object/components/component.h>
#include <scene/object/scripts/script.h>

#include <iostream>

// (smooth) operator overload for yaml serialisation
YAML::Emitter& operator<< (YAML::Emitter& serialiser, const Object& o) {
    // smooooooth operatorrrrrrrrrrr
    // smooooooooooooooooth operatorrrrrrrrrrrrrrr
    serialiser << YAML::BeginMap;
    // name
    serialiser << YAML::Key << "name";
    serialiser << YAML::Value << o.name;

    // components (as another sequence of maps)
    serialiser << YAML::Key << "components";
    serialiser << YAML::Value << YAML::BeginSeq;
    for (auto component : o.components)
        serialiser << *component; // expect map
    serialiser << YAML::Value << YAML::EndSeq;

    // scripts (as yet another seq of maps)
    serialiser << YAML::Key << "scripts";
    serialiser << YAML::Value << YAML::BeginSeq;
    for (auto script : o.scripts)
        serialiser << *script; // expect map
    serialiser << YAML::Value << YAML::EndSeq;

    // children (you can guess by now i hope)
    serialiser << YAML::Key << "children";
    serialiser << YAML::Value << YAML::BeginSeq;
    for (auto child : o.children)
        serialiser << *child; // expect map
    serialiser << YAML::Value << YAML::EndSeq;
    serialiser << YAML::EndMap;

    return serialiser;
}

std::shared_ptr<Object> Object::deserialise(const YAML::Node& objectNode, std::shared_ptr<Scene> s)
{
    std::shared_ptr<Object> obj(new Object(s));
    obj->name = objectNode["name"].as<std::string>();

    for (auto it = objectNode["components"].begin(); it != objectNode["components"].end(); ++it)
    {
        auto c = Component::deserialise(it->as<YAML::Node>(), obj);
        if (c)
            obj->components.push_back(c);
    }

    for (auto it = objectNode["scripts"].begin(); it != objectNode["scripts"].end(); ++it)
    {
        auto s = Script::deserialise(it->as<YAML::Node>(), obj);
        if (s)
            obj->scripts.push_back(s);
    }

    for (auto it = objectNode["children"].begin(); it != objectNode["children"].end(); ++it)
    {
        auto o = Object::deserialise(it->as<YAML::Node>(), s);
        o->parent = obj;
        if (o)
            obj->children.push_back(o);
    }

    return obj;
}

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

void Object::reparent(std::shared_ptr<Object> p, bool blueprint)
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
        if (!blueprint)
            for (auto it = scene->objects.begin(); it != scene->objects.end(); ++it) {
                if (it->get() == this)
                {
                    scene->objects.erase(it);
                    break;
                }
            }
        else
            for (auto it = scene->blueprints.begin(); it != scene->blueprints.end(); ++it) {
                if (it->get() == this)
                {
                    scene->blueprints.erase(it);
                    break;
                }
            }

    // are we just setting to null?
    if (p == nullptr) {
        parent.reset();
        parent = nullptr;
        if (!blueprint)
            scene->objects.push_back(shared_from_this()); // add back to scene direct child
        else
            scene->blueprints.push_back(shared_from_this());
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

void Object::remove()
{
    // remove all children
    for (auto child : children)
        if (child)
            child->remove();

    // remove all components
    for (auto component : components)
        if (component)
            component->remove();

    // 
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
}