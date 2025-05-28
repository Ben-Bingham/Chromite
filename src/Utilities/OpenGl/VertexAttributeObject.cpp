#include "VertexAttributeObject.h"

#include <gl/glew.h>
#include <GLFW/glfw3.h>

VertexAttributeObject::VertexAttributeObject() {
	glGenVertexArrays(1, &m_Handle);

	Bind();
}

VertexAttributeObject::~VertexAttributeObject() {
	glDeleteVertexArrays(1, &m_Handle);
}

void VertexAttributeObject::Bind() {
	glBindVertexArray(m_Handle);
}

void VertexAttributeObject::UnBind() {
	glBindVertexArray(0);
}