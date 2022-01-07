#pragma once

#include <glad/glad.h>

#include <string>
#include <memory>

class Shader : public std::enable_shared_from_this<Shader> {
public:
    GLuint id;
    std::string name;

    Shader(std::string name, const char* vertPath, const char* fragPath);
    ~Shader();

    void activate();
};