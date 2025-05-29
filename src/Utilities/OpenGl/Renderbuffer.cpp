#include "Renderbuffer.h"

Renderbuffer::Renderbuffer(glm::ivec2 size, InternalFormat internalFormat) {
    glGenRenderbuffers(1, &m_RenderbufferHandle);
    Bind();

    glRenderbufferStorage(GL_RENDERBUFFER, (int)internalFormat, size.x, size.y);
}

Renderbuffer::~Renderbuffer() {
    glDeleteRenderbuffers(1, &m_RenderbufferHandle);
}

void Renderbuffer::Bind() {
    glBindRenderbuffer(GL_RENDERBUFFER, m_RenderbufferHandle);
}

void Renderbuffer::Unbind() {
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

unsigned Renderbuffer::Get() {
    return m_RenderbufferHandle;
}
