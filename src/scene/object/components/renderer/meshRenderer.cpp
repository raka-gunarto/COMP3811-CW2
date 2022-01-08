#include "meshRenderer.h"

#include <scene/object/components/transform.h>
#include <scene/object/components/light.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <imgui.h>

#include <iostream>

void MeshRenderer::render(std::shared_ptr<Scene> s)
{
    // set default shader
    if (!shader)
        shader = object->getScene()->shaders[0];

    // find object transform
    std::shared_ptr<Transform> t = object->getComponent<Transform>();

    // cannot find transform, don't render anything
    if (t == nullptr)
    {
        std::cout << "WARN::" << object->getName() << "::meshRenderer::cannot find transform" << std::endl;
        return;
    }

    // activate the renderer's shader
    shader->activate();

    // model mat and normal mat
    glm::mat4 modelMat = t->modelMatrix();
    glm::mat3 normalMat = glm::mat3(glm::transpose(glm::inverse(modelMat)));
    glUniformMatrix4fv(glGetUniformLocation(shader->id, "model"), 1, GL_FALSE, glm::value_ptr(modelMat));
    glUniformMatrix3fv(glGetUniformLocation(shader->id, "normalMat"), 1, GL_FALSE, glm::value_ptr(normalMat));

    // load color / texture
    glUniform3f(glGetUniformLocation(shader->id, "diffuseColor"), mesh->diffuseColor.r, mesh->diffuseColor.g, mesh->diffuseColor.b);
    glUniform3f(glGetUniformLocation(shader->id, "specularColor"), mesh->specularColor.r, mesh->specularColor.g, mesh->specularColor.b);
    glUniform1f(glGetUniformLocation(shader->id, "shininess"), mesh->shininess);
    if (mesh->diffuseTex)
        mesh->diffuseTex->bind(GL_TEXTURE0);
    else {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    if (mesh->specularTex)
        mesh->specularTex->bind(GL_TEXTURE1);
    else {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    glUniform1i(glGetUniformLocation(shader->id, "diffuseTex"), 0);
    glUniform1i(glGetUniformLocation(shader->id, "specularTex"), 1);

    mesh->bind();
    glDrawElements(GL_TRIANGLES, mesh->indices(), GL_UNSIGNED_INT, (void*)0);
    mesh->unbind();
}


void MeshRenderer::renderInspector()
{
    ImGui::Text("Mesh Renderer");
    ImGui::Text("Shader: "); ImGui::SameLine();
    ImGui::Button(shader->name.c_str());
    if (ImGui::BeginDragDropTarget())
        if (const ImGuiPayload* p = ImGui::AcceptDragDropPayload("SHADER"))
            shader = (*((Shader**)p->Data))->shared_from_this();
    if (mesh)
        ImGui::Text(mesh->name.c_str());
    else ImGui::Text("Drop Mesh Here");
    if (ImGui::BeginDragDropTarget())
        if (const ImGuiPayload* p = ImGui::AcceptDragDropPayload("MESH"))
            mesh = (*(Mesh**)p->Data)->shared_from_this();
    ImGui::Separator();
}