#pragma once

#include <gl/glew.h>
#include <GLFW/glfw3.h>

#include "Texture.h"
#include "Renderbuffer.h"

class Framebuffer {
public:
    enum class TextureUses {
        COLOR_0 = GL_COLOR_ATTACHMENT0
    };

    enum class RenderbufferUses {
        DEPTH_STENCIL = GL_DEPTH_STENCIL_ATTACHMENT
    };

    Framebuffer();
    Framebuffer(const Framebuffer& other) = delete;
    Framebuffer(Framebuffer&& other) noexcept = default;
    Framebuffer& operator=(const Framebuffer& other) = delete;
    Framebuffer& operator=(Framebuffer&& other) noexcept = default;
    ~Framebuffer();

    void Bind();
    void Unbind();

    unsigned int Get();

    void AddTexture(Texture& texture, TextureUses use);
    void AddRenderbuffer(Renderbuffer& renderbuffer, RenderbufferUses use);

    bool Check();

private:
    unsigned int m_FramebufferHandle;
};