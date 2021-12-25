#include "shader.h"

#include <fstream>
#include <iostream>

char* readShaderFile(const char* path) {
    char* retbuf;

    std::ifstream f(path, std::ios::binary);
    f.seekg(0, std::ios::end);

    int len;
    len = f.tellg();
    retbuf = new char[len];
    f.seekg(0, std::ios::beg);
    f.read(retbuf, len);
    f.close();

    return retbuf;
}

void checkErrors(GLuint shader, GLenum type, const char* shaderPath)
{
    GLint compiled;
    char errors[1024];
    switch(type) {
        case GL_VERTEX_SHADER:
        case GL_FRAGMENT_SHADER:
            glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
            if (compiled == GL_FALSE) {
                glGetShaderInfoLog(shader, 1024, NULL, errors);
                std::cout << "ERROR::SHADER::COMPILE::" << shaderPath << "::" << errors << std::endl;
            }
        break;
        default:
            glGetProgramiv(shader, GL_COMPILE_STATUS, &compiled);
            if (compiled == GL_FALSE) {
                glGetShaderInfoLog(shader, 1024, NULL, errors);
                std::cout << "ERROR::SHADER::LINK::" << errors << std::endl;
            }
    }
}

Shader::Shader(const char* name, const char* vertFile, const char* fragFile)
{
    this->name = name;

    // read src
    const char* vertSrc = readShaderFile(vertFile);
    const char* fragSrc = readShaderFile(fragFile);

    // compile vertex shader
    GLuint vert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert, 1, &vertSrc, NULL);
    glCompileShader(vert);
    checkErrors(vert, GL_VERTEX_SHADER, vertFile);

    // comppile fragment shader
    GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag, 1, &fragSrc, NULL);
    glCompileShader(frag);
    checkErrors(frag, GL_FRAGMENT_SHADER, fragFile);

    id = glCreateProgram();
    glAttachShader(id, vert);
    glAttachShader(id, frag);
    glLinkProgram(id);
    checkErrors(id, 0, NULL);

    // cleanup
    delete[] (char*) vertSrc;
    delete[] (char*) fragSrc;
    glDeleteShader(vert);
    glDeleteShader(frag);
}

Shader::~Shader()
{
    glDeleteProgram(id);
}

void Shader::activate()
{
    glUseProgram(id);
}
