#include "mesh.h"

#include <memory>
#include <util/vao.h>
#include <util/vbo.h>
#include <util/ebo.h>

void Mesh::constructMesh(std::vector<GLfloat>& vertexData, std::vector<GLuint>& indices)
{
    vao = std::shared_ptr<VAO>(new VAO());
    vbo = std::shared_ptr<VBO>(new VBO(vao, &vertexData[0], vertexData.size() * sizeof(GLfloat)));
    ebo = std::shared_ptr<EBO>(new EBO(vao, &indices[0], indices.size() * sizeof(GLuint)));

    // set buffer attributes
    // vertex positions
    vao->link(vbo, 0, 3, GL_FLOAT, 8 * sizeof(GLfloat), (void*)0); // vertex coords
    // normals 
    vao->link(vbo, 1, 3, GL_FLOAT, 8 * sizeof(GLfloat), (void*)(3 * sizeof(float))); // vertex coords
    // texcoords 
    vao->link(vbo, 2, 2, GL_FLOAT, 8 * sizeof(GLfloat), (void*)(6 * sizeof(float))); // vertex coords
}

size_t Mesh::indices() { return ebo->size; }
void Mesh::bind() { vao->bind(); }
void Mesh::unbind() { vao->unbind(); }