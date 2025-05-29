#pragma once

#include <gl/glew.h>
#include <GLFW/glfw3.h>

class Framebuffer {
public:
    Framebuffer();
    Framebuffer(const Framebuffer& other) = delete;
    Framebuffer(Framebuffer&& other) noexcept = default;
    Framebuffer& operator=(const Framebuffer& other) = delete;
    Framebuffer& operator=(Framebuffer&& other) noexcept = default;
    ~Framebuffer();

    void Bind();
    void Unbind();

    unsigned int Get();

private:
    unsigned int m_FramebufferHandle;
};