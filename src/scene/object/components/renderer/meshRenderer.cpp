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

    // scene ambient parameters
    glUniform3f(glGetUniformLocation(shader->id, "ambientColor"),
        s->ambientColor.r,
        s->ambientColor.g,
        s->ambientColor.b
    );
    glUniform1f(glGetUniformLocation(shader->id, "ambientIntensity"), s->ambientIntensity);

    // model mat and normal mat
    glm::mat4 modelMat = t->modelMatrix();
    glm::mat3 normalMat = glm::mat3(glm::transpose(glm::inverse(modelMat)));
    glUniformMatrix4fv(glGetUniformLocation(shader->id, "model"), 1, GL_FALSE, glm::value_ptr(modelMat));
    glUniformMatrix3fv(glGetUniformLocation(shader->id, "normalMat"), 1, GL_FALSE, glm::value_ptr(normalMat));
    // specular lighting, camera position
    glUniform3f(glGetUniformLocation(shader->id, "cameraPos"),
        s->activeCamera->transform->worldPos().x,
        s->activeCamera->transform->worldPos().y,
        s->activeCamera->transform->worldPos().z
    );
    // camera matrix (view + projection)
    glUniformMatrix4fv(glGetUniformLocation(shader->id, "cameraMat"), 1, GL_FALSE, glm::value_ptr(s->activeCamera->getMatrix()));
    // lights
    for (int i = 0; i < s->lights.size(); ++i)
    {
        glUniform3f(glGetUniformLocation(shader->id, 
            std::string("lights[" + std::to_string(i) + "].pos").c_str()),
            s->lights[i]->transform->worldPos().x,
            s->lights[i]->transform->worldPos().y,
            s->lights[i]->transform->worldPos().z
            );
        glUniform3f(glGetUniformLocation(shader->id, 
            std::string("lights[" + std::to_string(i) + "].color").c_str()),
            s->lights[i]->color.r,
            s->lights[i]->color.g,
            s->lights[i]->color.b
            );
        glUniform1f(glGetUniformLocation(shader->id, 
            std::string("lights[" + std::to_string(i) + "].linAttenuate").c_str()),
            s->lights[i]->linearAttenuation
            );
        glUniform1f(glGetUniformLocation(shader->id, 
            std::string("lights[" + std::to_string(i) + "].quadAttenuate").c_str()),
            s->lights[i]->quadAttenuation
            );
    }
    // extract rotation from directional light transform
    if (s->dirLight)
    {
        glm::vec3 sunDirection = glm::rotate(
            glm::quat(s->dirLight->transform->modelMatrix()),
            glm::vec3(0, 1.0f, 0)
        );
        glUniform3f(glGetUniformLocation(shader->id, "sun.dir"),
            sunDirection.x,
            sunDirection.y,
            sunDirection.z
        );
        glUniform3f(glGetUniformLocation(shader->id, "sun.color"),
            s->dirLight->color.r,
            s->dirLight->color.g,
            s->dirLight->color.b
        );
    }

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