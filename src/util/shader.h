#pragma once

#include <glad/glad.h>

class Shader {
public:
    GLuint id;
    const char* name;

    Shader(const char* nane, const char* vertPath, const char* fragPath);
    ~Shader();

    void activate();
};