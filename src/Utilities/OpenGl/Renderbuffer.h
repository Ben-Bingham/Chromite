#pragma once

#include <gl/glew.h>
#include <GLFW/glfw3.h>

#include <glm/vec2.hpp>

class Renderbuffer {
public:
    enum class InternalFormat {
        DEPTH24_STENCIL8 = GL_DEPTH24_STENCIL8
    };

    Renderbuffer(glm::ivec2 size, InternalFormat internalFormat = InternalFormat::DEPTH24_STENCIL8);
    Renderbuffer(const Renderbuffer& other) = delete;
    Renderbuffer(Renderbuffer&& other) noexcept = default;
    Renderbuffer& operator=(const Renderbuffer& other) = delete;
    Renderbuffer& operator=(Renderbuffer&& other) noexcept = default;
    ~Renderbuffer();

    void Bind();
    void Unbind();

    unsigned int Get();

private:
    unsigned int m_RenderbufferHandle;
};