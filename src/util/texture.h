#pragma once

#include <glad/glad.h>

#include <stb_image.h>

#include <glm/glm.hpp>

#include <string>
#include <memory>

class Texture : public std::enable_shared_from_this<Texture> {
public:
    GLuint ID;
    std::string name;
    const GLenum type;
    const GLenum scaling;
    const GLenum repeat;

    struct ImageData {
        int width;
        int height;
        int colorChannels;
        GLenum format;
        GLenum pixelType;
        void* data;
        void (*free)(void*);
    };

    Texture(ImageData image, GLenum type, GLenum scaling, GLenum repeat, glm::vec4 borderColor = glm::vec4(1.0f), std::string name = "tex");
    ~Texture();

    void bind(GLenum slot);
    void unbind();

    static ImageData loadFromFile(const char* path, int format = STBI_rgb);
};