#pragma once

#include <glad/glad.h>

#include <memory>

class Texture;

class FBO {
public:
    GLuint id;
    const unsigned int width, height;
    const GLenum attachmentType;
    FBO(unsigned int width, unsigned int height, GLenum type);
    ~FBO();

    void bind();
    void unbind();

    std::shared_ptr<Texture> texture;
};