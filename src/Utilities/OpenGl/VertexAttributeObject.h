#pragma once

class VertexAttributeObject {
public:
	VertexAttributeObject();
    VertexAttributeObject(const VertexAttributeObject& other) = delete;
    VertexAttributeObject(VertexAttributeObject&& other) noexcept = default;
    VertexAttributeObject& operator=(const VertexAttributeObject& other) = delete;
    VertexAttributeObject& operator=(VertexAttributeObject&& other) noexcept = default;
    ~VertexAttributeObject();

    void Bind();
    void UnBind();

private:
    unsigned int m_Handle;
};