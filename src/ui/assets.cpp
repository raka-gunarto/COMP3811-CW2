#include "assets.h"

#include <imgui.h>

#include <scene/scene.h>

void Assets::render() {
    ImGui::Begin("Assets");

    // textures
    if (ImGui::CollapsingHeader("Textures"))
        if (ImGui::BeginTable("texture_tbl", 4))
        {
            for (auto tex : scene->textures)
            {
                ImGui::TableNextColumn();
                ImGui::PushID(tex.get());
                if (ImGui::BeginTable("tex", 1))
                {
                    ImGui::TableNextColumn();
                    ImGui::Image((void*)(intptr_t)tex->ID, ImVec2(100.0f, 100.0f), ImVec2(1, 1), ImVec2(0, 0));
                    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
                    {
                        Texture* ptr = tex.get();
                        ImGui::SetDragDropPayload("TEXTURE", &ptr, sizeof(Texture*));
                        ImGui::Text(std::string("TEXTURE:" + tex->name).c_str());
                        ImGui::EndDragDropSource();
                    }
                    ImGui::TableNextColumn();
                    ImGui::Text(tex->name.c_str());
                    ImGui::EndTable();
                }
                ImGui::PopID();
            }
            ImGui::EndTable();
        }

    ImGui::End();
}