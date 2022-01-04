#include "assets.h"

#include <imgui.h>

#include <scene/scene.h>
#include <scene/object/components/renderer/meshRenderer.h>

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

    // meshes
    if (ImGui::CollapsingHeader("Meshes"))
        if (ImGui::BeginTable("mesh_tbl", 4))
        {
            for (auto mesh : scene->meshes)
            {
                ImGui::TableNextColumn();
                ImGui::PushID(mesh.get());
                ImGui::Text(mesh->name.c_str());
                if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
                {
                    Mesh* ptr = mesh.get();
                    ImGui::SetDragDropPayload("MESH", &ptr, sizeof(Mesh*));
                    ImGui::Text(std::string("MESH:" + mesh->name).c_str());
                    ImGui::EndDragDropSource();
                }
                ImGui::PopID();
            }
            ImGui::EndTable();
        }

    // blueprints
    if (ImGui::CollapsingHeader("Blueprints"))
        if (ImGui::BeginTable("blueprint_tbl", 4))
        {
            for (auto blueprint : scene->blueprints)
            {
                ImGui::TableNextColumn();
                ImGui::PushID(blueprint.get());
                ImGui::Text(blueprint->getName().c_str());
                if(ImGui::SmallButton("Instantiate"))
                {
                    auto clone = blueprint->clone();
                    clone->reparent(nullptr); // reparent to scene
                }

                ImGui::PopID();
            }
            ImGui::EndTable();
        }

    ImGui::End();
}