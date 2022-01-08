#include "fbo.h"

#include <util/texture.h>

FBO::FBO(unsigned int width, unsigned int height, GLenum type)
    : width(width),
    height(height),
    attachmentType(type)
{
    // gen FBO
    glGenFramebuffers(1, &id);
    switch (type)
    {
    case GL_DEPTH_ATTACHMENT:
    {
        // generate texture
        texture = std::shared_ptr<Texture>(new Texture(width, height, GL_DEPTH_COMPONENT));
        texture->bind(GL_TEXTURE0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
        texture->unbind();

        // attach to fbo
        bind();
        glFramebufferTexture2D(GL_FRAMEBUFFER, type, GL_TEXTURE_2D, texture->ID, 0);
        // we don't care about colors, just want z-values
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        unbind();

        break;
    }
    default:
        break;
        // TODO: warn here or smth idk
    }
}
FBO::~FBO()
{
    glDeleteFramebuffers(1, &id);
}

void FBO::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, id);
}
void FBO::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}