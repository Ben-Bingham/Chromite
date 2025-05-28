#pragma once

#include <vector>
#include <iostream>

#include <gl/glew.h>
#include <GLFW/glfw3.h>

template<typename T, int BUFFER_TYPE>
class GlBuffer {
public:
    GlBuffer(const std::vector<T>& data, unsigned int drawMode = GL_STATIC_DRAW) {
        glGenBuffers(1, &m_Handle);

        Bind();

        glBufferData(BUFFER_TYPE, data.size() * sizeof(T), (void*)data.data(), drawMode);
    }

    GlBuffer(const GlBuffer& other) = delete;
    GlBuffer(GlBuffer&& other) noexcept = default;
    GlBuffer& operator=(const GlBuffer& other) = delete;
    GlBuffer& operator=(GlBuffer&& other) noexcept = default;

    ~GlBuffer() {
        glDeleteBuffers(1, &m_Handle);
    }

    void Bind() {
        glBindBuffer(BUFFER_TYPE, m_Handle);
    }

private:
    unsigned int m_Handle{ 0 };
};

using VertexBufferObject = GlBuffer<float, GL_ARRAY_BUFFER>;
using ElementBufferObject = GlBuffer<unsigned int, GL_ELEMENT_ARRAY_BUFFER>;
