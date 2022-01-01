#include "script.h"

#include <scene/object/scripts/camera/editCamera.h>

#include <imgui.h>

#include <memory>

template <typename T>
Script* newComponent(std::shared_ptr<Object> obj) { return new T(obj); }

struct ScriptBuilders {
    const char* name;
    Script* (*builder)(std::shared_ptr<Object>);
};

static ScriptBuilders builders[] = {
    {"Edit Camera Controls", newComponent<EditCamera>},
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