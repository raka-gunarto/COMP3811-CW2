#include "planeRenderer.h"

#include <scene/scene.h>
#include <scene/object/components/transform.h>

#include <imgui.h>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <memory>

bool PlaneRenderer::initialised = false;
std::shared_ptr<VBO> PlaneRenderer::planeVBO;
std::shared_ptr<VAO> PlaneRenderer::planeVAO;
std::shared_ptr<EBO> PlaneRenderer::planeEBO;

PlaneRenderer::PlaneRenderer(std::shared_ptr<Object> obj) : Renderer(obj)
{
    // ensure vertex data is initialised
    PlaneRenderer::initVertexData();

    name = "PlaneRenderer";

    // default mode flat color, white
    mode = PlaneRenderer::Mode::MATERIAL;
    diffuseColor = glm::vec3(1.0f, 1.0f, 1.0f);

    // set default shader
    shader = obj->getScene()->shaders[0];
}

void PlaneRenderer::initVertexData() {
    if (initialised) return;
    // initialise buffers
    // the vertices can be constant, it can be transformed using
    // the transform component. Vertices are for a flat plane
    GLfloat verts[] = {
        // four vertices of the plane
        -1.0f, 0, -1.0f,
        -1.0f, 0, 1.0f,
        1.0f, 0, -1.0f,
        1.0f, 0, 1.0f,
    };
    GLuint elements[] = {
        0,1,2, // one half of plane
        2,1,3 // other half of plane (maintain CCW winding order)
    };
    planeVAO = std::shared_ptr<VAO>(new VAO());
    planeVBO = std::shared_ptr<VBO>(new VBO(planeVAO, verts, sizeof(verts)));
    planeEBO = std::shared_ptr<EBO>(new EBO(planeVAO, elements, sizeof(elements)));

    // set buffer attributes
    planeVAO->link(planeVBO, 0, 3, GL_FLOAT, 3 * sizeof(GLfloat), (void*)0); // vertex coords

    initialised = true;
}

void PlaneRenderer::render(std::shared_ptr<Scene> s)
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
        std::cout << "WARN::" << object->getName() << "::planeRenderer::cannot find transform" << std::endl;
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
    case PlaneRenderer::Mode::MATERIAL:
        glUniform3f(glGetUniformLocation(shader->id, "diffuseColor"),
            diffuseColor.r,
            diffuseColor.g,
            diffuseColor.b
        );
        glUniform1f(glGetUniformLocation(shader->id, "specularIntensity"), specularIntensity);
        break;
    case PlaneRenderer::Mode::TEX_MAP:
        glUniform3f(glGetUniformLocation(shader->id, "diffuseColor"), 0, 0, 0);
        glUniform1f(glGetUniformLocation(shader->id, "specularIntensity"), 0);
        break;
    }
    planeVAO->bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
    planeVAO->unbind();
}

void PlaneRenderer::renderInspector()
{
    ImGui::Text("Plane Renderer");
    ImGui::Text("Mode"); ImGui::SameLine();
    if (ImGui::RadioButton("Material", mode == MATERIAL)) { mode = MATERIAL; } ImGui::SameLine();
    if (ImGui::RadioButton("Texture Map", mode == TEX_MAP)) { mode = TEX_MAP; }
    switch (mode) {
    case PlaneRenderer::Mode::MATERIAL:
        ImGui::ColorEdit3("Diffuse", glm::value_ptr(diffuseColor));
        break;
    case PlaneRenderer::Mode::TEX_MAP:
        if (ImGui::BeginDragDropTarget())
            ImGui::EndDragDropTarget();
        break;
    }
    ImGui::Separator();
}