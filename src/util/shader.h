#pragma once

#include <glad/glad.h>

#include <string>

class Shader {
public:
    GLuint id;
    std::string name;

    Shader(std::string name, const char* vertPath, const char* fragPath);
    ~Shader();

    void activate();
};