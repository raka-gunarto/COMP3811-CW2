#include "planeRenderer.h"

#include <scene/object/components/transform.h>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

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
    mode = PlaneRenderer::Mode::FLAT_COLOR;
    color = glm::vec3(1.0f, 1.0f, 1.0f);
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
        1,2,3 // other half of plane
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

    // model mat and normal mat
    glm::mat4 modelMat = t->modelMatrix();
    glm::mat3 normalMat = glm::mat3(glm::transpose(glm::inverse(modelMat)));
    glUniformMatrix4fv(glGetUniformLocation(shader->id, "model"), 1, GL_FALSE, glm::value_ptr(modelMat));
    glUniformMatrix3fv(glGetUniformLocation(shader->id, "normalMat"), 1, GL_FALSE, glm::value_ptr(normalMat));
    // mode: color / texture?
    glUniform1i(glGetUniformLocation(shader->id, "mode"), (uint)mode);
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
    case PlaneRenderer::Mode::FLAT_COLOR:
        glUniform3f(glGetUniformLocation(shader->id, "color"),
            color.r,
            color.g,
            color.b
        );
        break;
    case PlaneRenderer::Mode::TEXTURE:
        break;
    }
    planeVAO->bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
    planeVAO->unbind();
}

void PlaneRenderer::renderInspector()
{

}