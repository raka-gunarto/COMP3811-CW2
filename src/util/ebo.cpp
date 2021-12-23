#include "ebo.h"

EBO::EBO(std::shared_ptr<VAO> vao, GLuint* indices, GLsizeiptr sz, GLenum use)
{
    vao->bind();
    
    glGenBuffers(1, &id);
    bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sz, indices, GL_STATIC_DRAW);

    vao->unbind();
    unbind();
}

EBO::~EBO()
{
    glDeleteBuffers(1, &id);
}

void EBO::bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

void EBO::unbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}