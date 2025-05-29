#include "Framebuffer.h"

Framebuffer::Framebuffer() {
    glGenFramebuffers(1, &m_FramebufferHandle);
    Bind();
}

Framebuffer::~Framebuffer() {
    glDeleteFramebuffers(1, &m_FramebufferHandle);
}

void Framebuffer::Bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferHandle);
}

void Framebuffer::Unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned Framebuffer::Get() {
    return m_FramebufferHandle;
}

void Framebuffer::AddTexture(Texture& texture, TextureUses use) {
    glFramebufferTexture2D(GL_FRAMEBUFFER, (int)use, GL_TEXTURE_2D, texture.Get(), 0);
}

void Framebuffer::AddRenderbuffer(Renderbuffer& renderbuffer, RenderbufferUses use) {
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, (int)use, GL_RENDERBUFFER, renderbuffer.Get());
}

bool Framebuffer::Check() {
    Bind();

    return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}