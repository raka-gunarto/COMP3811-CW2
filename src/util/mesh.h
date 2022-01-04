#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>

#include <string>
#include <memory>
#include <vector>

class VAO; class EBO; class VBO;
class Texture;

class Mesh : public std::enable_shared_from_this<Mesh> {
public:
    std::string name;
    // for now this function expects the vertex data to be laid as follows
    // layout (location = 0) in vec3 aPos;
    // layout (location = 1) in vec3 aNormal;
    // layout (location = 2) in vec2 aTexCoords;
    void constructMesh(std::vector<GLfloat>& vertexData, std::vector<GLuint>& indices);
    size_t indices();
    void bind();
    void unbind();

    glm::vec3 diffuseColor = glm::vec3(0.0f);
    glm::vec3 specularColor = glm::vec3(0.0f);
    float shininess = 1.0f;
    std::shared_ptr<Texture> diffuseTex = nullptr;
    std::shared_ptr<Texture> specularTex = nullptr;
private:
    std::shared_ptr<VAO> vao;
    std::shared_ptr<VBO> vbo;
    std::shared_ptr<EBO> ebo;
};