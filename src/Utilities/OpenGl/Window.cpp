#include "Window.h"

#include <iostream>

Window::Window(glm::ivec2 size, const std::string& name) 
	: size(size) {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

    handle = glfwCreateWindow(size.x, size.y, name.c_str(), NULL, NULL);
    if (handle == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }

    glfwSwapInterval(0);
}

Window::~Window() {
    glfwTerminate();

}