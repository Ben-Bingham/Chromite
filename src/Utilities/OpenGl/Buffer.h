#pragma once

#include <vector>
#include <iostream>

#include <gl/glew.h>
#include <GLFW/glfw3.h>

template<typename T, int BUFFER_TYPE>
class GlBuffer {
public:
    GlBuffer(const std::vector<T>& data, unsigned int drawMode = GL_STATIC_DRAW) 
        : m_DrawMode(drawMode), m_BufferSize(data.size()) {
        glGenBuffers(1, &m_Handle);

        Bind();

        glBufferData(BUFFER_TYPE, data.size() * sizeof(T), (void*)data.data(), m_DrawMode);
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

    void UpdateData(const std::vector<T>& data, size_t offset = 0) {
        if (data.size() + offset <= m_BufferSize) {
            glBufferSubData(BUFFER_TYPE, offset, data.size(), (void*)data.data());
        }
        else {
            glBufferData(BUFFER_TYPE, data.size() * sizeof(T), (void*)data.data(), m_DrawMode);

            m_BufferSize = data.size();
        }
    }

    void ReplaceData(const std::vector<T>& data) {
        glBufferData(BUFFER_TYPE, data.size() * sizeof(T), (void*)data.data(), m_DrawMode);

        m_BufferSize = data.size();
    }

private:
    unsigned int m_Handle{ 0 };

    unsigned int m_DrawMode;

    size_t m_BufferSize;
};

using VertexBufferObject = GlBuffer<float, GL_ARRAY_BUFFER>;
using ElementBufferObject = GlBuffer<unsigned int, GL_ELEMENT_ARRAY_BUFFER>;
