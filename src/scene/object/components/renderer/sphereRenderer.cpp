#include "sphereRenderer.h"

#include <scene/scene.h>
#include <scene/object/components/transform.h>
#include <scene/object/components/light.h>

#include <imgui.h>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include <iostream>
#include <memory>

bool SphereRenderer::initialised = false;
std::shared_ptr<VBO> SphereRenderer::sphereVBO;
std::shared_ptr<VAO> SphereRenderer::sphereVAO;
std::shared_ptr<EBO> SphereRenderer::sphereEBO;

SphereRenderer::SphereRenderer(std::shared_ptr<Object> obj) : Renderer(obj)
{
    // ensure vertex data is initialised
    SphereRenderer::initVertexData();

    name = "SphereRenderer";

    // default mode flat color, white
    mode = SphereRenderer::Mode::MATERIAL;
    diffuseColor = glm::vec3(1.0f, 1.0f, 1.0f);
    specularColor = glm::vec3(0.0f, 0.0f, 0.0f);
}

void SphereRenderer::initVertexData() {
    if (initialised) return;
    // initialise buffers
    // the vertices can be constant, it can be transformed using
    // the transform component. Vertices calculated are for a sphere

    // calculate steps for sectors and stacks 
    // note: glm::pi is a constexpr, will be evaluated at compile-time
    std::vector<GLfloat> verts;
    std::vector<GLuint> elements;
    const float sectorStep = (float)2 * glm::pi<float>() / (float)SECTORS;
    const float stackStep = glm::pi<float>() / (float)STACKS;

    for (int stack = 0; stack <= STACKS; ++stack)
    {
        float stackAngle = ((glm::pi<float>() / 2) - (stack * stackStep));
        float xyComponent = glm::cos(stackAngle);
        float z = glm::sin(stackAngle);

        unsigned int stackStart = stack * (SECTORS + 1);
        unsigned int stackNext = stackStart + SECTORS + 1;

        for (int sector = 0; sector <= SECTORS; ++sector, ++stackStart, ++stackNext)
        {
            float sectorAngle = (sector * sectorStep);
            float
                x = xyComponent * glm::cos(sectorAngle),
                y = xyComponent * glm::sin(sectorAngle);

            // positions
            verts.push_back(x);
            verts.push_back(-z);
            verts.push_back(-y);

            // normals
            verts.push_back(x);
            verts.push_back(-z);
            verts.push_back(-y);

            // texcoords
            verts.push_back((float)sector / SECTORS);
            verts.push_back((float)stack / STACKS);

            // indices
            // stackStart +---+ stackStart + 1
            //            |  /|
            //            |/  |
            // startNext  +---+ stackNext + 1 
            if (stack == STACKS || sector == SECTORS) continue;
            if (stack != 0)
            {
                elements.push_back(stackStart);
                elements.push_back(stackNext);
                elements.push_back(stackStart + 1);
            }
            if (stack != (STACKS - 1))
            {
                elements.push_back(stackStart + 1);
                elements.push_back(stackNext);
                elements.push_back(stackNext + 1);
            }
        }
    }

    sphereVAO = std::shared_ptr<VAO>(new VAO());
    sphereVBO = std::shared_ptr<VBO>(new VBO(sphereVAO, &verts[0], verts.size() * sizeof(GLfloat)));
    sphereEBO = std::shared_ptr<EBO>(new EBO(sphereVAO, &elements[0], elements.size() * sizeof(GLuint)));

    // set buffer attributes
    // vertex positions
    sphereVAO->link(sphereVBO, 0, 3, GL_FLOAT, 8 * sizeof(GLfloat), (void*)0); // vertex coords
    // normals 
    sphereVAO->link(sphereVBO, 1, 3, GL_FLOAT, 8 * sizeof(GLfloat), (void*)(3 * sizeof(float))); // vertex coords
    // texcoords 
    sphereVAO->link(sphereVBO, 2, 2, GL_FLOAT, 8 * sizeof(GLfloat), (void*)(6 * sizeof(float))); // vertex coords

    initialised = true;
}

void SphereRenderer::render(std::shared_ptr<Scene> s, std::shared_ptr<Shader> shaderOverride)
{
    // set default shader
    if (!shader)
        shader = object->getScene()->shaders[0];

    // find object transform
    std::shared_ptr<Transform> t = object->getComponent<Transform>();

    // cannot find transform, don't render anything
    if (t == nullptr)
    {
        std::cout << "WARN::" << object->getName() << "::sphereRenderer::cannot find transform" << std::endl;
        return;
    }

    // activate the renderer's shader
    if (shaderOverride)
        shaderOverride->activate();
    else
        shader->activate();

    // model mat and normal mat
    glm::mat4 modelMat = t->modelMatrix();
    glm::mat3 normalMat = glm::mat3(glm::transpose(glm::inverse(modelMat)));
    glUniformMatrix4fv(glGetUniformLocation(shader->id, "model"), 1, GL_FALSE, glm::value_ptr(modelMat));
    glUniformMatrix3fv(glGetUniformLocation(shader->id, "normalMat"), 1, GL_FALSE, glm::value_ptr(normalMat));

    // load color / texture
    switch (mode)
    {
    case SphereRenderer::Mode::MATERIAL:
        glUniform3f(glGetUniformLocation(shader->id, "diffuseColor"),
            diffuseColor.r,
            diffuseColor.g,
            diffuseColor.b
        );
        glUniform3f(glGetUniformLocation(shader->id, "specularColor"),
            specularColor.r,
            specularColor.g,
            specularColor.b
        );
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, 0);
        glUniform1f(glGetUniformLocation(shader->id, "shininess"), shininess);
        break;
    case SphereRenderer::Mode::TEX_MAP:
        glUniform3f(glGetUniformLocation(shader->id, "diffuseColor"), 0, 0, 0);
        glUniform3f(glGetUniformLocation(shader->id, "specularColor"), 0, 0, 0);
        glUniform1f(glGetUniformLocation(shader->id, "shininess"), shininess);
        if (diffuseTex)
            diffuseTex->bind(GL_TEXTURE0);
        else {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        if (specularTex)
            specularTex->bind(GL_TEXTURE1);
        else {
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        glUniform1i(glGetUniformLocation(shader->id, "diffuseTex"), 0);
        glUniform1i(glGetUniformLocation(shader->id, "specularTex"), 1);
        break;
    }
    sphereVAO->bind();
    glDrawElements(GL_TRIANGLES, sphereEBO->size, GL_UNSIGNED_INT, (void*)0);
    sphereVAO->unbind();
}

void SphereRenderer::renderInspector()
{
    ImGui::Text("Sphere Renderer");
    ImGui::Text("Shader: "); ImGui::SameLine();
    ImGui::Button(shader->name.c_str());
    if (ImGui::BeginDragDropTarget())
        if (const ImGuiPayload* p = ImGui::AcceptDragDropPayload("SHADER"))
            shader = (*((Shader**)p->Data))->shared_from_this();

    ImGui::Text("Mode"); ImGui::SameLine();
    if (ImGui::RadioButton("Material", mode == MATERIAL)) { mode = MATERIAL; } ImGui::SameLine();
    if (ImGui::RadioButton("Texture Map", mode == TEX_MAP)) { mode = TEX_MAP; }
    switch (mode) {
    case SphereRenderer::Mode::MATERIAL:
        ImGui::ColorEdit3("Diffuse", glm::value_ptr(diffuseColor));
        ImGui::ColorEdit3("Specular", glm::value_ptr(specularColor));
        ImGui::DragFloat("Shininess", &shininess, 0.1f, 0.0f);
        break;
    case SphereRenderer::Mode::TEX_MAP:
        ImGui::Text("Diffuse Map");
        if (diffuseTex)
            ImGui::Image((void*)(intptr_t)diffuseTex->ID, ImVec2(50.0f, 50.0f), ImVec2(1, 1), ImVec2(0, 0));
        else
            ImGui::Button("Drop Texture Here", ImVec2(50.0f, 50.0f));
        if (ImGui::BeginDragDropTarget())
            if (const ImGuiPayload* p = ImGui::AcceptDragDropPayload("TEXTURE"))
                diffuseTex = (*(Texture**)p->Data)->shared_from_this();

        ImGui::NewLine();

        ImGui::Text("Specular Map");
        if (specularTex)
            ImGui::Image((void*)(intptr_t)specularTex->ID, ImVec2(50.0f, 50.0f), ImVec2(1, 1), ImVec2(0, 0));
        else
            ImGui::Button("Drop Texture Here", ImVec2(50.0f, 50.0f));
        if (ImGui::BeginDragDropTarget())
            if (const ImGuiPayload* p = ImGui::AcceptDragDropPayload("TEXTURE"))
                specularTex = (*(Texture**)p->Data)->shared_from_this();
        ImGui::DragFloat("Shininess", &shininess, 0.1f, 0.0f);
        break;
    }
    ImGui::Separator();
}