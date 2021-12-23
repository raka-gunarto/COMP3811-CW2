#pragma once

#include <glad/glad.h>

#include <memory>

#include "vbo.h"

class VBO;

class VAO {
public:
    GLuint id;
    VAO();
    ~VAO();

    void link(std::shared_ptr<VBO> vbo, GLuint layout, GLuint nComponents, GLenum type, GLsizeiptr stride, void* offset);
    void bind();
    void unbind();
};