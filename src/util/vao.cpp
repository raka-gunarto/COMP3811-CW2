#include "vao.h"

VAO::VAO()
{
    glGenVertexArrays(1, &id);
}

void VAO::link(std::shared_ptr<VBO> vbo, GLuint layout, GLuint nComponents, GLenum type, GLsizeiptr stride, void* offset) {
    bind();
    vbo->bind();

    glVertexAttribPointer(layout, nComponents, type, GL_FALSE, stride, offset);
    glEnableVertexAttribArray(layout);

    unbind();
    vbo->unbind();
}

VAO::~VAO() {
    glDeleteVertexArrays(1, &id);
}

void VAO::bind()
{
    glBindVertexArray(id);
}

void VAO::unbind()
{
    glBindVertexArray(0);
}