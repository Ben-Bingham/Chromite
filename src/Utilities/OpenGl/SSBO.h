#pragma once

#include <vector>

#include <gl/glew.h>
#include <GLFW/glfw3.h>

template<typename T>
class SSBO {
public:
    SSBO(unsigned int bufferBase, const std::vector<T>& data = std::vector<T>{ }) {
        glGenBuffers(1, &m_Handle);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_Handle);

        if (data.empty()) {
            glBufferData(GL_SHADER_STORAGE_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
        }
        else {
            glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(T) * data.size(), data.data(), GL_DYNAMIC_DRAW);
        }

        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bufferBase, m_Handle);

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }

    ~SSBO() {
        glDeleteBuffers(1, &m_Handle);
    }

    SSBO(const SSBO& other) = delete;
    SSBO(SSBO&& other) noexcept = default;
    SSBO& operator=(const SSBO& other) = delete;
    SSBO& operator=(SSBO&& other) noexcept = default;

    void SetData(const std::vector<T>& data) {
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_Handle);
        glBufferData(GL_SHADER_STORAGE_BUFFER, (GLsizeiptr)(data.size() * sizeof(T)), data.data(), GL_DYNAMIC_DRAW);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }

private:
    unsigned int m_Handle;
};
