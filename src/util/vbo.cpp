#include "vbo.h"

// constructor generates a buffer and copies vertices into
// the buffer. It does a copy so verts does not have to be
// a dynamic array
VBO::VBO(std::shared_ptr<VAO> vao, GLfloat* verts, GLsizeiptr sz, GLenum use)
{
    vao->bind();
    // generate buffer, bind, copy verts, unbind
    glGenBuffers(1, &id);
    bind();
    glBufferData(GL_ARRAY_BUFFER, sz, verts, use);

    vao->unbind();
    unbind();
}

VBO::~VBO() {
    glDeleteBuffers(1, &id);
}

void VBO::bind() {
    glBindBuffer(GL_ARRAY_BUFFER, id);
}

void VBO::unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}