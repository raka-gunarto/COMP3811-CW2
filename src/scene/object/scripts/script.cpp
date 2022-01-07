#include "script.h"

#include <scene/object/scripts/camera/editCamera.h>
#include <scene/object/scripts/sunMoonCycle.h>

#include <imgui.h>

#include <memory>

template <typename T>
Script* newComponent(std::shared_ptr<Object> obj) { return new T(obj); }

struct ScriptBuilders {
    const char* name;
    Script* (*builder)(std::shared_ptr<Object>);
};

static ScriptBuilders builders[] = {
    {"EditCamera", newComponent<EditCamera>},
    {"SunMoonCycle", newComponent<SunMoonCycle>},
};

void Script::renderComponentChildWindow(std::shared_ptr<Object> obj) {
    static bool open = true;
    if (ImGui::BeginPopupModal("Script Menu###inspectorScriptMenu", &open))
    {
        // FIXME: not sure if there's a dynamic way to do this,
        //        short of making each component a dll and dynamically
        //        reading symbols or smth
        for (auto cb : builders)
        {
            if (ImGui::Button(cb.name))
            {
                obj->scripts.push_back(std::shared_ptr<Script>(cb.builder(obj)));
                ImGui::CloseCurrentPopup();
            }
            ImGui::Separator();
        }
        ImGui::SetWindowSize(ImVec2(0, 0));
        ImGui::EndPopup();
    }
    if (ImGui::Button("Add Script"))
        ImGui::OpenPopup("Script Menu###inspectorScriptMenu");
}

std::shared_ptr<Script> Script::deserialise(const YAML::Node& scriptNode, std::shared_ptr<Object> obj)
{
    // find the corresponding builder
    if (scriptNode.IsMap())
        for (auto cb : builders)
            if (scriptNode["name"].as<std::string>() == std::string(cb.name))
            {
                auto script = std::shared_ptr<Script>(cb.builder(obj));
                script->_deserialise(scriptNode);
                return script;
            }

    return nullptr;
}

void Script::remove()
{
    // find self in obj components and remove (shared_ptr should automatically free us)
    for (auto it = object->scripts.begin(); it != object->scripts.end(); ++it) {
        if (it->get() == this)
        {
            object->scripts.erase(it);
            break;
        }
    }
}