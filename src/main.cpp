#include <iostream>
#include <fstream>
#include <algorithm>

#include <glm/gtc/matrix_transform.hpp>

#include "imgui.h"

#include "Utilities/OpenGl/Context.h"
#include "Utilities/OpenGl/Shader.h"
#include "Utilities/OpenGl/Texture.h"
#include "Utilities/OpenGl/SSBO.h"
#include "Utilities/ImGui/ImGuiInstance.h"
#include "Utilities/OpenGl/VertexAttributeObject.h"
#include "Utilities/OpenGl/Buffer.h"
#include "Utilities/Camera.h"

void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

std::shared_ptr<Window> window{ };

int main() {
    window = std::make_shared<Window>(glm::ivec2{ 1600, 1000 }, "Chromite");

    glfwSetFramebufferSizeCallback(window->handle, FramebufferSizeCallback);

    Context context{ *window };
   
    ImGuiInstance imGui{ };
    imGui.Init(window->handle);

    float lastFrame = 0.0f;

    while (!glfwWindowShouldClose(window->handle)) {
        float currentFrame = (float)glfwGetTime();

        float dt = currentFrame - lastFrame;
        lastFrame = currentFrame;

        imGui.StartNewFrame();
        
        // Show GUI
        { ImGui::Begin("Viewport");
        ImGui::Text("Hello World!");
        } ImGui::End();

        if (glfwGetKey(window->handle, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window->handle, true);
        }

        // Prep for rendering
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Finish the GUI frame
        imGui.FinishFrame();

        // Finish the frame
        glfwSwapBuffers(window->handle);
        glfwPollEvents();
    }

    imGui.Cleanup();
}

void FramebufferSizeCallback(GLFWwindow* w, int width, int height) {
    glViewport(0, 0, width, height);
    
    window->size.x = width;
    window->size.y = height;
}
