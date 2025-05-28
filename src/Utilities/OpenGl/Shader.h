#pragma once

#include <string>

#include <glm/glm.hpp>

class Shader {
public:
    Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const std::string& geometryShaderPath = "");
    Shader(const Shader& other) = delete;
    Shader(Shader&& other) noexcept = default;
    Shader& operator=(const Shader& other) = delete;
    Shader& operator=(Shader&& other) noexcept = default;
    ~Shader();

    void Bind();

    void SetFloat(const std::string& name, const float& value);
    void SetInt  (const std::string& name, const int& value);

    void SetVec3(const std::string& name, const glm::vec3& value);
    void SetVec4(const std::string& name, const glm::vec4& value);
    void SetMat4(const std::string& name, const glm::mat4& value);

    void SetBool(const std::string& name, const bool& value);

private:
    unsigned int m_ShaderHandle;
};
