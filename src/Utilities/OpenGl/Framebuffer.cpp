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
