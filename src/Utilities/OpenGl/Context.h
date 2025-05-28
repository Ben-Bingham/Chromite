#pragma once

#include "Window.h"

#include <gl/glew.h>
#include <GLFW/glfw3.h>

class Context {
public:
	Context(const Window& window);
	Context(const Context& other) = delete;
	Context(Context&& other) noexcept = default;
	Context& operator=(const Context& other) = delete;
	Context& operator=(Context&& other) noexcept = default;
	~Context();
};