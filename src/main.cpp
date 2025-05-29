#include <iostream>
#include <fstream>
#include <algorithm>

#include <glm/gtc/matrix_transform.hpp>

#include "imgui.h"

#include "Utilities/OpenGl/Context.h"
#include "Utilities/OpenGl/Shader.h"
#include "Utilities/OpenGl/Texture.h"
#include "Utilities/OpenGl/Framebuffer.h"
#include "Utilities/OpenGl/Renderbuffer.h"
#include "Utilities/OpenGl/SSBO.h"
#include "Utilities/ImGui/ImGuiInstance.h"
#include "Utilities/OpenGl/VertexAttributeObject.h"
#include "Utilities/OpenGl/Buffer.h"
#include "Utilities/Camera.h"

void MouseMovementCallback(GLFWwindow* window, double x, double y);

Camera cam{ };

std::shared_ptr<Window> window{ };

glm::ivec2 imGuiWindowSize{ };
glm::ivec2 lastImGuiWindowSize{ };

int main() {
    window = std::make_shared<Window>(glm::ivec2{ 1600, 1000 }, "Chromite");

    glfwSetCursorPosCallback(window->handle, MouseMovementCallback);

    Context context{ *window };

    ImGuiInstance imGui{ };
    imGui.Init(window->handle);

    Shader mainShader{ "assets\\shaders\\main.vert", "assets\\shaders\\main.frag" };
    mainShader.Bind();
    mainShader.SetVec4("color", glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f });

    std::vector<float> vertices{
        -0.5f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.0f,
        0.0f, 0.5f, 0.0f
    };

    std::vector<unsigned int> indices{
        1, 2, 3
    };

    VertexAttributeObject vao{ };

    VertexBufferObject vbo{ vertices };
    ElementBufferObject ebo{ indices };

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(0);

    Framebuffer framebuffer{ };

    Texture texture{ window->size, Texture::Parameters{ 
        Texture::Format::RGB, 
        Texture::StorageType::UNSIGNED_BYTE, 
        Texture::WrapMode::REPEAT,
        Texture::FilteringMode::LINEAR 
    } };

    framebuffer.AddTexture(texture, Framebuffer::TextureUses::COLOR_0);
    
    Renderbuffer renderbuffer{ window->size };

    framebuffer.AddRenderbuffer(renderbuffer, Framebuffer::RenderbufferUses::DEPTH_STENCIL);

    if (!framebuffer.Check()) {
        std::cout << "ERROR, Framebuffer is not complete" << std::endl;
    }

    framebuffer.Unbind();
    texture.Unbind();
    renderbuffer.Unbind();

    float lastFrame = 0.0f;

    while (!glfwWindowShouldClose(window->handle)) {
        float currentFrame = (float)glfwGetTime();

        float dt = currentFrame - lastFrame;
        lastFrame = currentFrame;

        imGui.StartNewFrame();

        //ImGui::ShowDemoWindow();

        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
        
        // Show GUI
        { ImGui::Begin("Viewport");
            imGuiWindowSize = glm::ivec2{ ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y};

            if (imGuiWindowSize != lastImGuiWindowSize) {
                framebuffer.Bind();

                texture.Bind();
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imGuiWindowSize.x, imGuiWindowSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.Get(), 0);

                glViewport(0, 0, imGuiWindowSize.x, imGuiWindowSize.y);

                renderbuffer.Bind();
                glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, imGuiWindowSize.x, imGuiWindowSize.y);
                glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbuffer.Get());

                framebuffer.Unbind();
            }

            ImGui::Image((ImTextureID)texture.Get(), ImVec2{ (float)imGuiWindowSize.x, (float)imGuiWindowSize.y });

        } ImGui::End();

        { ImGui::Begin("Info");
            ImGui::Text("Frame time %3.4f", dt);
        } ImGui::End();

        if (glfwGetKey(window->handle, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window->handle, true);
        }

        // Camera Movement
        if (glfwGetKey(window->handle, GLFW_KEY_W) == GLFW_PRESS) {
            cam.position += cam.forward * cam.movementSpeed * dt;
        }
        if (glfwGetKey(window->handle, GLFW_KEY_S) == GLFW_PRESS) {
            cam.position -= cam.forward * cam.movementSpeed * dt;
        }
        if (glfwGetKey(window->handle, GLFW_KEY_A) == GLFW_PRESS) {
            cam.position -= cam.right * cam.movementSpeed * dt;
        }
        if (glfwGetKey(window->handle, GLFW_KEY_D) == GLFW_PRESS) {
            cam.position += cam.right * cam.movementSpeed * dt;
        }
        if (glfwGetKey(window->handle, GLFW_KEY_SPACE) == GLFW_PRESS) {
            cam.position += cam.up * cam.movementSpeed * dt;
        }
        if (glfwGetKey(window->handle, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            cam.position -= cam.up * cam.movementSpeed * dt;
        }

        framebuffer.Bind();

        // Prep for rendering
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model{ 1.0f };
        glm::mat4 view = cam.ViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(60.0f), (float)imGuiWindowSize.x / (float)imGuiWindowSize.y, 0.01f, 100.0f);

        glm::mat4 mvp = projection * view * model;

        mainShader.Bind();
        mainShader.SetMat4("mvp", mvp);

        // Render
        glDrawElements(GL_TRIANGLES, (unsigned int)indices.size(), GL_UNSIGNED_INT, nullptr);

        framebuffer.Unbind();

        // Finish the GUI frame
        imGui.FinishFrame();

        // Finish the frame
        glfwSwapBuffers(window->handle);
        glfwPollEvents();

        lastImGuiWindowSize = imGuiWindowSize;
    }

    imGui.Cleanup();
}

void MouseMovementCallback(GLFWwindow* window, double x, double y) {
    //glm::vec2 pos{ (float)x, (float)y };

    //if (cam.lastMousePos.x == std::numeric_limits<float>::max()) {
    //    cam.lastMousePos = pos;
    //}

    //glm::vec2 posOffset{ pos.x - cam.lastMousePos.x, cam.lastMousePos.y - pos.y };

    //cam.lastMousePos = pos;

    //posOffset *= cam.lookSensitivity;

    //cam.yaw += posOffset.x;
    //cam.pitch += posOffset.y;

    //cam.pitch = std::clamp(cam.pitch, -89.0f, 89.0f);

    //cam.forward.x = cos(glm::radians(cam.yaw)) * cos(glm::radians(cam.pitch));
    //cam.forward.y = sin(glm::radians(cam.pitch));
    //cam.forward.z = sin(glm::radians(cam.yaw)) * cos(glm::radians(cam.pitch));
    //cam.forward = glm::normalize(cam.forward);

    //cam.right = glm::normalize(glm::cross(cam.forward, cam.up));
}
