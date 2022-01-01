#include "sphereRenderer.h"

#include <scene/scene.h>
#include <scene/object/components/transform.h>

#include <imgui.h>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

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

    // set default shader
    shader = obj->getScene()->shaders[0];
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
            verts.push_back(y);
            verts.push_back(z);

            // normals
            verts.push_back(x);
            verts.push_back(y);
            verts.push_back(z);

            // TODO: texcoords

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
    sphereEBO = std::shared_ptr<EBO>(new EBO(sphereVAO, &elements[0], elements.size() * sizeof(GLfloat)));

    // set buffer attributes
    // vertex positions
    sphereVAO->link(sphereVBO, 0, 3, GL_FLOAT, 6 * sizeof(GLfloat), (void*)0); // vertex coords
    // normals 
    sphereVAO->link(sphereVBO, 1, 3, GL_FLOAT, 6 * sizeof(GLfloat), (void*)(3 * sizeof(float))); // vertex coords

    initialised = true;
}

void SphereRenderer::render(std::shared_ptr<Scene> s)
{
    // find object transform
    std::shared_ptr<Transform> t = nullptr;
    for (auto component : object->components)
        if (component->getName() == "Transform")
        {
            t = std::dynamic_pointer_cast<Transform>(component);
            break;
        }

    // cannot find transform, don't render anything
    if (t == nullptr)
    {
        std::cout << "WARN::" << object->getName() << "::sphereRenderer::cannot find transform" << std::endl;
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
        s->activeCamera->transform->position.x,
        s->activeCamera->transform->position.y,
        s->activeCamera->transform->position.z
    );
    // camera matrix (view + projection)
    glUniformMatrix4fv(glGetUniformLocation(shader->id, "cameraMat"), 1, GL_FALSE, glm::value_ptr(s->activeCamera->getMatrix()));
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
        glUniform1f(glGetUniformLocation(shader->id, "shininess"), shininess);
        break;
    case SphereRenderer::Mode::TEX_MAP:
        glUniform3f(glGetUniformLocation(shader->id, "diffuseColor"), 0, 0, 0);
        glUniform1f(glGetUniformLocation(shader->id, "specularIntensity"), 0);
        break;
    }
    sphereVAO->bind();
    glDrawElements(GL_TRIANGLES, sphereEBO->size, GL_UNSIGNED_INT, (void*)0);
    sphereVAO->unbind();
}

void SphereRenderer::renderInspector()
{
    ImGui::Text("Sphere Renderer");
    ImGui::Text("Mode"); ImGui::SameLine();
    if (ImGui::RadioButton("Material", mode == MATERIAL)) { mode = MATERIAL; } ImGui::SameLine();
    if (ImGui::RadioButton("Texture Map", mode == TEX_MAP)) { mode = TEX_MAP; }
    switch (mode) {
    case SphereRenderer::Mode::MATERIAL:
        ImGui::ColorEdit3("Diffuse", glm::value_ptr(diffuseColor));
        ImGui::ColorEdit3("Specular", glm::value_ptr(specularColor));
        ImGui::SliderFloat("Shininess", &shininess, 0.0f, 1.0f);
        break;
    case SphereRenderer::Mode::TEX_MAP:
        // if (ImGui::BeginDragDropTarget())
        //     ImGui::EndDragDropTarget();
        ImGui::SliderFloat("Shininess", &shininess, 0.0f, 1.0f);
        break;
    }
    ImGui::Separator();
}