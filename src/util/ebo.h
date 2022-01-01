#pragma once

#include <util/vao.h>

#include <glad/glad.h>

#include <memory>

class VAO;

class EBO {
public:
    GLuint id;
    size_t size;
    EBO(std::shared_ptr<VAO> vao, GLuint* indices, GLsizeiptr sz, GLenum use = GL_STATIC_DRAW);
    ~EBO();

    void bind();
    void unbind();
};