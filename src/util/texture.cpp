#include "texture.h"

#include <glm/gtc/type_ptr.hpp>

Texture::Texture(ImageData image, GLenum type, GLenum scaling, GLenum repeat, glm::vec4 borderColor, std::string name)
    : type(type),
    scaling(scaling),
    repeat(repeat),
    name(name)
{
    // generate texture
    glGenTextures(1, &ID);
    bind(GL_TEXTURE0);

    // set parameters, process texture and generate mipmaps
    if (scaling == GL_LINEAR)
        glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    else if (scaling == GL_NEAREST)
        glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    else
        glTexParameteri(type, GL_TEXTURE_MIN_FILTER, scaling);
    glTexParameteri(type, GL_TEXTURE_MAG_FILTER, scaling);
    glTexParameteri(type, GL_TEXTURE_WRAP_S, repeat);
    glTexParameteri(type, GL_TEXTURE_WRAP_T, repeat);
    if (repeat == GL_CLAMP_TO_BORDER)
        glTexParameterfv(type, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(borderColor));
    glTexImage2D(type, 0, GL_RGBA, image.width, image.height, 0, image.format, image.pixelType, image.data);
    glGenerateMipmap(type);

    // free original buffer now that opengl has it
    if (image.free != nullptr)
        image.free(image.data);
    else
        delete (unsigned char*)image.data;
    unbind();
}

Texture::ImageData Texture::loadFromFile(const char* path, int format)
{
    Texture::ImageData d;
    d.pixelType = GL_UNSIGNED_BYTE;

    stbi_set_flip_vertically_on_load(true);
    d.data = stbi_load(path, &d.width, &d.height, &d.colorChannels, format);
    if (format == 3)
        d.format = GL_RGB;
    else if (format == 4)
        d.format = GL_RGBA;
    else
        d.format = GL_RED;
    d.free = [](void* data) {
        stbi_image_free((stbi_uc*)data);
    };
    return d;
}

Texture::Texture(unsigned int width, unsigned int height, GLenum fmt, std::string name)
    : name(name),
    scaling(GL_NEAREST),
    repeat(GL_CLAMP_TO_BORDER),
    type(GL_TEXTURE_2D)
{
    // generate texture
    glGenTextures(1, &ID);
    bind(GL_TEXTURE0);

    // params
    glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 0.0, 0.0, 0.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glTexImage2D(GL_TEXTURE_2D, 0, fmt, width, height, 0, fmt, GL_FLOAT, NULL);
    unbind();
}

Texture::~Texture()
{
    glDeleteTextures(1, &ID);
}

void Texture::bind(GLenum slot)
{
    glActiveTexture(slot);
    glBindTexture(type, ID);
}

void Texture::unbind()
{
    glBindTexture(type, 0);
}