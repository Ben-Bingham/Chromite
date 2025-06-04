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

#include "Grid.h"

#include "Components/Emitter.h"
#include "Components/StraightWire.h"
#include "Components/Printer.h"

#define DEBUG

void MouseMovementCallback(GLFWwindow* window, double x, double y);
glm::ivec2 windowMousePosition{ };
glm::ivec2 gridMousePosition{ };

void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
bool updateGrid = false;
float zoom{ 1.0f };

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
bool dragging = false;
Chromite::Component* draggedComponent{ nullptr };
glm::vec2 mouseOffsetWhenGrabbed{ 0.0f };

glm::ivec2 viewportOffset{ };

float scrollSensitivity = 0.1f;

Camera cam{ 1.0f };

std::shared_ptr<Window> window{ };

glm::ivec2 imGuiWindowSize{ };
glm::ivec2 lastImGuiWindowSize{ };

Chromite::Grid grid{ };

std::vector<Chromite::Component*> components{ };

int x = 0;
int y = 0;

int main() {
    components.resize(3);

    components[0] = new Chromite::Emitter();
    components[1] = new Chromite::StraightWire();
    components[1]->position = glm::ivec2{ 1, 0 };
    components[2] = new Chromite::Printer();
    components[2]->position = glm::ivec2{ 2, 0 };

    components[0]->east = components[1];
    components[1]->west = components[0];
    components[1]->east = components[2];
    components[2]->west = components[1];

    window = std::make_shared<Window>(glm::ivec2{ 1600, 1000 }, "Chromite");

    glfwSetCursorPosCallback(window->handle, MouseMovementCallback);
    glfwSetScrollCallback(window->handle, ScrollCallback);
    glfwSetMouseButtonCallback(window->handle, MouseButtonCallback);

    glfwSwapInterval(1);

    Context context{ *window };

    ImGuiInstance imGui{ };
    imGui.Init(window->handle);

    Shader mainShader{ "assets\\shaders\\main.vert", "assets\\shaders\\main.frag" };
    Shader atlasShader{ "assets\\shaders\\atlas.vert", "assets\\shaders\\atlas.frag" };

#ifdef DEBUG
    // North
    std::vector<float> northVertices {
        grid.gridLength / 4.0f, -grid.gridLength / 4.0f, 0.0f,
        grid.gridLength / 4.0f, grid.gridLength / 4.0f, 0.0f
    };

    std::vector<unsigned int> northIndices{
        0, 1
    };

    VertexAttributeObject northVao{ };

    VertexBufferObject northVbo{ northVertices };
    ElementBufferObject northEbo{ northIndices };

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(0);

    // East
    std::vector<float> eastVertices{
        grid.gridLength - (grid.gridLength / 4.0f), grid.gridLength / 4.0f, 0.0f,
        grid.gridLength + (grid.gridLength / 4.0f), grid.gridLength / 4.0f, 0.0f
    };

    std::vector<unsigned int> eastIndices{
        0, 1
    };

    VertexAttributeObject eastVao{ };

    VertexBufferObject eastVbo{ eastVertices };
    ElementBufferObject eastEbo{ eastIndices };

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(0);

    // South
    std::vector<float> southVertices{
        grid.gridLength / 4.0f * 3.0f, grid.gridLength - (grid.gridLength / 4.0f), 0.0f,
        grid.gridLength / 4.0f * 3.0f, grid.gridLength + (grid.gridLength / 4.0f), 0.0f
    };

    std::vector<unsigned int> southIndices{
        0, 1
    };

    VertexAttributeObject southVao{ };

    VertexBufferObject southVbo{ southVertices };
    ElementBufferObject southEbo{ southIndices };

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(0);

    // West
    std::vector<float> westVertices{
        -grid.gridLength / 4.0f, grid.gridLength / 4.0f * 3.0f, 0.0f,
        grid.gridLength / 4.0f, grid.gridLength / 4.0f * 3.0f, 0.0f
    };

    std::vector<unsigned int> westIndices{
        0, 1
    };

    VertexAttributeObject westVao{ };

    VertexBufferObject westVbo{ westVertices };
    ElementBufferObject westEbo{ westIndices };

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(0);
#endif

    TextureParameters atlasParams{ };
    atlasParams.magFilter = TextureFilteringMode::NEAREST;
    atlasParams.minFilter = TextureFilteringMode::NEAREST;

    Texture2D_Array atlas{ std::vector<std::string>{
        "assets\\sprites\\straight-wire.png",
        "assets\\sprites\\emitter-one-sided.png",
        "assets\\sprites\\light-one-sided.png"
    }, atlasParams };

    std::vector<float> vertices{
        0.0f,               grid.gridLength,    0.0f,       0.0f, 1.0f,
        grid.gridLength,    grid.gridLength,    0.0f,       1.0f, 1.0f,
        grid.gridLength,    0.0f,               0.0f,       1.0f, 0.0f,
        0.0f,               0.0f,               0.0f,       0.0f, 0.0f
    };

    std::vector<unsigned int> indices{
        0, 1, 2,
        2, 3, 0
    };

    VertexAttributeObject vao{ };

    VertexBufferObject vbo{ vertices };
    ElementBufferObject ebo{ indices };

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    Chromite::Grid::MeshData meshData = grid.GenerateMeshData();

    VertexAttributeObject gridVAO{ };
    VertexBufferObject gridVBO{ meshData.vertices };
    ElementBufferObject gridEBO{ meshData.indices };

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(0);

    Framebuffer framebuffer{ };

    Texture2D texture{ window->size, TextureParameters{
        TextureFormat::RGB, 
        TextureStorageType::UNSIGNED_BYTE, 
        TextureWrapMode::REPEAT,
        TextureFilteringMode::LINEAR 
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

    int counter = 0;

    while (!glfwWindowShouldClose(window->handle)) {
        float currentFrame = (float)glfwGetTime();

        float dt = currentFrame - lastFrame;
        lastFrame = currentFrame;

        if (counter == 60) {
            dynamic_cast<Chromite::Emitter*>(components[0])->Emit();

            counter = 0;
        }

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

            viewportOffset = glm::ivec2{ (int)ImGui::GetCursorPos().x, (int)ImGui::GetCursorPos().y };
            ImGui::Image((ImTextureID)texture.Get(), ImVec2{ (float)imGuiWindowSize.x, (float)imGuiWindowSize.y });

        } ImGui::End();

        { ImGui::Begin("Info");
            ImGui::Text("Frame time %3.4f", dt);

            glm::ivec2 mousePosition = windowMousePosition - viewportOffset;

            ImGui::Text("Mouse Position (%d, %d)", mousePosition.x, mousePosition.y);

            glm::mat4 view = cam.ViewMatrix();
            float aspect = (float)imGuiWindowSize.x / (float)imGuiWindowSize.y;
            glm::mat4 projection = glm::ortho(-aspect / zoom, aspect / zoom, -1.0f / zoom, 1.0f / zoom, 0.01f, 100.0f);

            glm::vec4 viewport{ 0.0f, 0.0f, imGuiWindowSize.x, imGuiWindowSize.y };

            glm::vec3 mousePositionWorldspace = glm::unProject(glm::vec3{ (float)mousePosition.x, (float)mousePosition.y, 1.0f }, view, projection, viewport);

            ImGui::Text("Mouse Position World Space (%.3f, %.3f)", mousePositionWorldspace.x, mousePositionWorldspace.y);
  
            gridMousePosition = glm::floor(mousePositionWorldspace / grid.gridLength);

            ImGui::Text("Mouse Grid Position (%d, %d)", gridMousePosition.x, gridMousePosition.y);

        } ImGui::End();

        { ImGui::Begin("Settings");
            if (ImGui::DragInt2("Grid Size", &grid.gridSize.x, 0.1f)) updateGrid = true;
            if (ImGui::DragFloat("Grid Width", &grid.n, 0.001f)) updateGrid = true;
            if (ImGui::DragFloat("Grid Square Size", &grid.gridLength, 0.001f)) updateGrid = true;

            ImGui::Separator();

            ImGui::DragFloat("Scroll Sensitivity", &scrollSensitivity, 0.0001f, 0.0f, 1.0f);
        } ImGui::End();

        if (updateGrid) {
            Chromite::Grid::MeshData meshData = grid.GenerateMeshData();

            gridVAO.Bind();
            gridVBO.ReplaceData(meshData.vertices);
            gridEBO.ReplaceData(meshData.indices);

            gridVAO.UnBind();
            gridVBO.UnBind();
            gridEBO.UnBind();

            vertices = std::vector<float>{
                0.0f,               grid.gridLength,    0.0f,       0.0f, 1.0f,
                grid.gridLength,    grid.gridLength,    0.0f,       1.0f, 1.0f,
                grid.gridLength,    0.0f,               0.0f,       1.0f, 0.0f,
                0.0f,               0.0f,               0.0f,       0.0f, 0.0f
            };

            vao.Bind();
            vbo.ReplaceData(vertices);

            vao.UnBind();
            vbo.UnBind();
            ebo.UnBind();

#ifdef DEBUG
            // North
            northVertices = std::vector<float>{
                grid.gridLength / 4.0f, -grid.gridLength / 4.0f, 0.0f,
                grid.gridLength / 4.0f, grid.gridLength / 4.0f, 0.0f
            };

            northVao.Bind();
            northVbo.ReplaceData(northVertices);

            northVao.UnBind();
            northVbo.UnBind();
            northEbo.UnBind();

            // East
            eastVertices = std::vector<float>{
                grid.gridLength - (grid.gridLength / 4.0f), grid.gridLength / 4.0f, 0.0f,
                grid.gridLength + (grid.gridLength / 4.0f), grid.gridLength / 4.0f, 0.0f
            };

            eastVao.Bind();
            eastVbo.ReplaceData(eastVertices);

            eastVao.UnBind();
            eastVbo.UnBind();
            eastEbo.UnBind();

            // South
            southVertices = std::vector<float>{
                grid.gridLength / 4.0f * 3.0f, grid.gridLength - (grid.gridLength / 4.0f), 0.0f,
                grid.gridLength / 4.0f * 3.0f, grid.gridLength + (grid.gridLength / 4.0f), 0.0f
            };

            southVao.Bind();
            southVbo.ReplaceData(southVertices);

            southVao.UnBind();
            southVbo.UnBind();
            southEbo.UnBind();

            // West
            westVertices = std::vector<float>{
                -grid.gridLength / 4.0f, grid.gridLength / 4.0f * 3.0f, 0.0f,
                grid.gridLength / 4.0f, grid.gridLength / 4.0f * 3.0f, 0.0f
            };

            westVao.Bind();
            westVbo.ReplaceData(westVertices);

            westVao.UnBind();
            westVbo.UnBind();
            westEbo.UnBind();
#endif

            updateGrid = false;
        }

        if (glfwGetKey(window->handle, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window->handle, true);
        }

        if (glfwGetKey(window->handle, GLFW_KEY_A) == GLFW_PRESS) {
            cam.position -= cam.right * cam.movementSpeed * dt;
        }
        if (glfwGetKey(window->handle, GLFW_KEY_D) == GLFW_PRESS) {
            cam.position += cam.right * cam.movementSpeed * dt;
        }
        if (glfwGetKey(window->handle, GLFW_KEY_S) == GLFW_PRESS) {
            cam.position += cam.up * cam.movementSpeed * dt;
        }
        if (glfwGetKey(window->handle, GLFW_KEY_W) == GLFW_PRESS) {
            cam.position -= cam.up * cam.movementSpeed * dt;
        }

        framebuffer.Bind();

        // Prep for rendering
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = cam.ViewMatrix();
        float aspect = (float)imGuiWindowSize.x / (float)imGuiWindowSize.y;
        glm::mat4 projection = glm::ortho(-aspect / zoom, aspect / zoom, -1.0f / zoom, 1.0f / zoom, 0.01f, 100.0f);

        mainShader.Bind();

        // Render

        // Grid goes first to appear behind everthing
        glm::mat4 model{ 1.0f };
        glm::mat4 mvp = projection * view * model;

        mainShader.SetMat4("mvp", mvp);

        mainShader.SetVec4("color", glm::vec4{ 0.6f, 0.6f, 0.6f, 1.0f });

        gridVAO.Bind();
        glDrawElements(GL_TRIANGLES, (unsigned int)grid.indexCount, GL_UNSIGNED_INT, nullptr);

        atlasShader.Bind();

        atlas.ActivateUnit(0);
        atlas.Bind();
        atlasShader.SetInt("atlas", 0);

        for (auto& component : components) {
            glm::mat4 model{ 1.0f };

            if (component == draggedComponent) {
                glm::ivec2 mousePosition = windowMousePosition - viewportOffset;
                glm::vec4 viewport{ 0.0f, 0.0f, imGuiWindowSize.x, imGuiWindowSize.y };

                glm::vec3 mousePositionWorldspace = glm::unProject(glm::vec3{ (float)mousePosition.x, (float)mousePosition.y, 1.0f }, view, projection, viewport);

                model = glm::translate(model, mousePositionWorldspace);
                model = glm::translate(model, glm::vec3{ -mouseOffsetWhenGrabbed.x, -mouseOffsetWhenGrabbed.y, 0.0f });
            }
            else {
                model = glm::translate(model, glm::vec3{ component->position.x * grid.gridLength, component->position.y * grid.gridLength, 0.0f });
            }

            glm::mat4 mvp = projection * view * model;
            atlasShader.SetMat4("mvp", mvp);

            Chromite::Emitter* emitter = dynamic_cast<Chromite::Emitter*>(component);
            Chromite::StraightWire* straightWire = dynamic_cast<Chromite::StraightWire*>(component);
            Chromite::Printer* printer = dynamic_cast<Chromite::Printer*>(component);

            if (emitter != nullptr) {
                atlasShader.SetVec4("color", glm::vec4{ 1.0f, 1.0f, 0.0f, 1.0f });
            }

            if (straightWire != nullptr) {
                atlasShader.SetVec4("color", glm::vec4{ 0.0f, 1.0f, 1.0f, 1.0f });
            }

            if (printer != nullptr) {
                atlasShader.SetVec4("color", glm::vec4{ 0.0f, 0.0f, 1.0f, 1.0f });
            }

            vao.Bind();
            glDrawElements(GL_TRIANGLES, (unsigned int)indices.size(), GL_UNSIGNED_INT, nullptr);
        }

        mainShader.Bind();

#ifdef DEBUG // Render DEBUG last so that it appears above everything else
        for (auto& component : components) {
            glm::mat4 model{ 1.0f };

            if (component == draggedComponent) {
                glm::ivec2 mousePosition = windowMousePosition - viewportOffset;
                glm::vec4 viewport{ 0.0f, 0.0f, imGuiWindowSize.x, imGuiWindowSize.y };

                glm::vec3 mousePositionWorldspace = glm::unProject(glm::vec3{ (float)mousePosition.x, (float)mousePosition.y, 1.0f }, view, projection, viewport);

                model = glm::translate(model, mousePositionWorldspace);
                model = glm::translate(model, glm::vec3{ -mouseOffsetWhenGrabbed.x, -mouseOffsetWhenGrabbed.y, 0.0f });
            }
            else {
                model = glm::translate(model, glm::vec3{ component->position.x * grid.gridLength, component->position.y * grid.gridLength, 0.0f });
            }
            glm::mat4 mvp = projection * view * model;
            mainShader.SetMat4("mvp", mvp);

            // North
            mainShader.SetVec4("color", glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f });
            if (component->north == nullptr) mainShader.SetVec4("color", glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f });

            northVao.Bind();
            glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, nullptr);

            // East
            mainShader.SetVec4("color", glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f });
            if (component->east == nullptr) mainShader.SetVec4("color", glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f });
            
            eastVao.Bind();
            glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, nullptr);

            // South
            mainShader.SetVec4("color", glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f });
            if (component->south == nullptr) mainShader.SetVec4("color", glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f });

            southVao.Bind();
            glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, nullptr);

            // West
            mainShader.SetVec4("color", glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f });
            if (component->west == nullptr) mainShader.SetVec4("color", glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f });

            westVao.Bind();
            glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, nullptr);
        }
#endif

        framebuffer.Unbind();

        // Finish the GUI frame
        imGui.FinishFrame();

        // Finish the frame
        glfwSwapBuffers(window->handle);
        glfwPollEvents();

        lastImGuiWindowSize = imGuiWindowSize;

        ++counter;
    }

    imGui.Cleanup();

    for (auto component : components) {
        delete component;
    }
}

void MouseMovementCallback(GLFWwindow* window, double x, double y) {
    windowMousePosition.x = (int)x;
    windowMousePosition.y = (int)y;
}

void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    zoom += (float)yoffset * scrollSensitivity;

    if (zoom < 0.5f) zoom = 0.5f;
    if (zoom > 4.0f) zoom = 4.0f;

    updateGrid = true;
}

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS) {
        for (auto& component : components) {
            if (component->position == gridMousePosition) {
                draggedComponent = component;
                dragging = true;

                glm::ivec2 mousePosition = windowMousePosition - viewportOffset;

                glm::mat4 view = cam.ViewMatrix();
                float aspect = (float)imGuiWindowSize.x / (float)imGuiWindowSize.y;
                glm::mat4 projection = glm::ortho(-aspect / zoom, aspect / zoom, -1.0f / zoom, 1.0f / zoom, 0.01f, 100.0f);

                glm::vec4 viewport{ 0.0f, 0.0f, imGuiWindowSize.x, imGuiWindowSize.y };

                glm::vec3 mousePositionWorldspace = glm::unProject(glm::vec3{ (float)mousePosition.x, (float)mousePosition.y, 1.0f }, view, projection, viewport);

                gridMousePosition = glm::floor(mousePositionWorldspace / grid.gridLength);

                mouseOffsetWhenGrabbed = glm::vec2{ mousePositionWorldspace - glm::vec3{ grid.gridLength * component->position.x, grid.gridLength * component->position.y, 0.0f } };

                break;
            }
        }
    }

    if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_RELEASE) {
        if (!dragging) return;

        glm::ivec2 mousePosition = windowMousePosition - viewportOffset;

        glm::mat4 view = cam.ViewMatrix();
        float aspect = (float)imGuiWindowSize.x / (float)imGuiWindowSize.y;
        glm::mat4 projection = glm::ortho(-aspect / zoom, aspect / zoom, -1.0f / zoom, 1.0f / zoom, 0.01f, 100.0f);

        glm::vec4 viewport{ 0.0f, 0.0f, imGuiWindowSize.x, imGuiWindowSize.y };

        glm::vec3 mousePositionWorldspace = glm::unProject(glm::vec3{ (float)mousePosition.x, (float)mousePosition.y, 1.0f }, view, projection, viewport);

        glm::ivec2 newPos = glm::floor(mousePositionWorldspace / grid.gridLength);

        if (draggedComponent->north != nullptr) draggedComponent->north->south = nullptr;
        if (draggedComponent->east != nullptr) draggedComponent->east->west = nullptr;
        if (draggedComponent->south != nullptr) draggedComponent->south->north = nullptr;
        if (draggedComponent->west != nullptr) draggedComponent->west->east = nullptr;

        draggedComponent->position = newPos;

        glm::ivec2 newNorthPos = newPos + glm::ivec2{ 0, -1 };
        glm::ivec2 newEastPos = newPos + glm::ivec2{ 1, 0 };
        glm::ivec2 newSouthPos = newPos + glm::ivec2{ 0, 1 };
        glm::ivec2 newWestPos = newPos + glm::ivec2{ -1, 0 };

        draggedComponent->north = nullptr;
        draggedComponent->east = nullptr;
        draggedComponent->south = nullptr;
        draggedComponent->west = nullptr;

        auto newNorthIt = std::find_if(components.begin(), components.end(), [&newNorthPos](Chromite::Component* component) { return component->position == newNorthPos; });
        auto newEastIt = std::find_if(components.begin(), components.end(), [&newEastPos](Chromite::Component* component) { return component->position == newEastPos; });
        auto newSouthIt = std::find_if(components.begin(), components.end(), [&newSouthPos](Chromite::Component* component) { return component->position == newSouthPos; });
        auto newWestIt = std::find_if(components.begin(), components.end(), [&newWestPos](Chromite::Component* component) { return component->position == newWestPos; });

        if (newNorthIt != components.end()) { draggedComponent->north = *newNorthIt; (*newNorthIt)->south = draggedComponent; }
        if (newEastIt != components.end()) { draggedComponent->east = *newEastIt; (*newEastIt)->west = draggedComponent; }
        if (newSouthIt != components.end()) { draggedComponent->south = *newSouthIt; (*newSouthIt)->north = draggedComponent; }
        if (newWestIt != components.end()) { draggedComponent->west = *newWestIt; (*newWestIt)->east = draggedComponent; }

        dragging = false;
        draggedComponent = nullptr;

        mouseOffsetWhenGrabbed = glm::vec2{ 0.0f };
    }
}