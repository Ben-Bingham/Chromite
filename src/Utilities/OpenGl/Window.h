#pragma once

#include <string>

#include <gl/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

class Window {
public:
	Window(glm::ivec2 size, const std::string& name = "Window");
	Window(const Window& other) = delete;
	Window(Window&& other) noexcept = default;
	Window& operator=(const Window& other) = delete;
	Window& operator=(Window&& other) noexcept = default;
	~Window();

	GLFWwindow* handle;

	glm::ivec2 size;
};