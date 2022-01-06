#include "component.h"

#include <scene/object/object.h>

#include <scene/object/components/transform.h>
#include <scene/object/components/camera.h>
#include <scene/object/components/light.h>
#include <scene/object/components/renderer/meshRenderer.h>
#include <scene/object/components/renderer/planeRenderer.h>
#include <scene/object/components/renderer/cubeRenderer.h>
#include <scene/object/components/renderer/sphereRenderer.h>

#include <imgui.h>

#include <memory>

template <typename T>
Component* newComponent(std::shared_ptr<Object> obj) { return new T(obj); }

struct ComponentBuilders {
    const char* name;
    Component* (*builder)(std::shared_ptr<Object>);
};

static ComponentBuilders builders[] = {
    {"Transform", newComponent<Transform>},
    {"Camera", newComponent<Camera>},
    {"Light", newComponent<Light>},
    {"Plane Renderer", newComponent<PlaneRenderer>},
    {"Cube Renderer", newComponent<CubeRenderer>},
    {"Sphere Renderer", newComponent<SphereRenderer>},
    {"Mesh Renderer", newComponent<MeshRenderer>},
};

void Component::renderComponentChildWindow(std::shared_ptr<Object> obj) {
    static bool open = true;
    if (ImGui::BeginPopupModal("Component Menu###inspectorComponentMenu", &open))
    {
        // FIXME: not sure if there's a dynamic way to do this,
        //        short of making each component a dll and dynamically
        //        reading symbols or smth
        for (auto cb : builders)
        {
            if (ImGui::Button(cb.name))
            {
                obj->components.push_back(std::shared_ptr<Component>(cb.builder(obj)));
                ImGui::CloseCurrentPopup();
            }
            ImGui::Separator();
        }
        ImGui::SetWindowSize(ImVec2(0, 0));
        ImGui::EndPopup();
    }
    if (ImGui::Button("Add Component"))
    {
        open = true;
        ImGui::OpenPopup("Component Menu###inspectorComponentMenu");
    }
}

void Component::remove()
{
    // find self in obj components and remove (shared_ptr should automatically free us)
    for (auto it = object->components.begin(); it != object->components.end(); ++it) {
        if (it->get() == this)
        {
            object->components.erase(it);
            break;
        }
    }
}