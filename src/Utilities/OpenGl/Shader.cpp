#include "Shader.h"

#include <gl/glew.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <iostream>
#include <sstream>

#include <glm/gtc/type_ptr.hpp>
#include "Utilities/ReadFile.h"

Shader::Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const std::string& geometryShaderPath) {
    std::string vertexShaderSource = ReadFile(vertexShaderPath);
    const char* vertexSource = vertexShaderSource.c_str();

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, nullptr);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cout << "ERROR: Vertex shader failed to compile:" << std::endl;
        std::cout << infoLog << std::endl;
    }

    std::string fragmentShaderSource = ReadFile(fragmentShaderPath);
    const char* fragmentSource = fragmentShaderSource.c_str();

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cout << "ERROR: Fragment shader failed to compile:" << std::endl;
        std::cout << infoLog << std::endl;
    }

    unsigned int geometryShader = 0;
    if (!geometryShaderPath.empty()) {
        std::string geometryShaderSource = ReadFile(geometryShaderPath);
        const char* geometrySource = geometryShaderSource.c_str();

        geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometryShader, 1, &geometrySource, nullptr);
        glCompileShader(geometryShader);

        glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(geometryShader, 512, nullptr, infoLog);
            std::cout << "ERROR: Geometry shader failed to compile:" << std::endl;
            std::cout << infoLog << std::endl;
        }
    }

    m_ShaderHandle = glCreateProgram();
    glAttachShader(m_ShaderHandle, vertexShader);
    glAttachShader(m_ShaderHandle, fragmentShader);

    if (!geometryShaderPath.empty()) {
        glAttachShader(m_ShaderHandle, geometryShader);
    }

    glLinkProgram(m_ShaderHandle);

    glGetProgramiv(m_ShaderHandle, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(m_ShaderHandle, 512, nullptr, infoLog);
        std::cout << "ERROR: Shader program failed to link:" << std::endl;
        std::cout << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    if (!geometryShaderPath.empty()) {
        glDeleteShader(geometryShader);
    }
}

Shader::~Shader() {
    glDeleteProgram(m_ShaderHandle);
}

void Shader::Bind() {
    glUseProgram(m_ShaderHandle);
}

void Shader::SetFloat(const std::string& name, const float& value) {
    glUniform1f(glGetUniformLocation(m_ShaderHandle, name.c_str()), value);
}

void Shader::SetInt(const std::string& name, const int& value) {
    glUniform1i(glGetUniformLocation(m_ShaderHandle, name.c_str()), value);
}

void Shader::SetVec3(const std::string& name, const glm::vec3& value) {
    glUniform3fv(glGetUniformLocation(m_ShaderHandle, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::SetVec4(const std::string& name, const glm::vec4& value) {
    glUniform4fv(glGetUniformLocation(m_ShaderHandle, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::SetMat4(const std::string& name, const glm::mat4& value) {
    glUniformMatrix4fv(glGetUniformLocation(m_ShaderHandle, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetBool(const std::string& name, const bool& value) {
    int realValue = 0;
    if (value) {
        realValue = 1;
    }

    SetInt(name, realValue);
}
