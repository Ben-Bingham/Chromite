#pragma once
#include <vector>
#include <string>
#include <iostream>

#include <gl/glew.h>
#include <GLFW/glfw3.h>

#include <glm/vec2.hpp>
#include <stb_image.h>

enum class TextureFormat {
    R = GL_RED,
    RGB = GL_RGB,
    RGBA = GL_RGBA
};

enum class TextureStorageType {
    UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
    FLOAT = GL_FLOAT
};

enum class TextureWrapMode {
    REPEAT = GL_REPEAT
};

enum class TextureFilteringMode {
    LINEAR = GL_LINEAR,
    NEAREST = GL_NEAREST
};

struct TextureParameters {
    TextureParameters(
        TextureFormat imageFormat = TextureFormat::RGBA,
        TextureStorageType internalStorageType = TextureStorageType::UNSIGNED_BYTE,
        TextureWrapMode wrapMode = TextureWrapMode::REPEAT,
        TextureFilteringMode filteringMode = TextureFilteringMode::LINEAR
    )
        : imageFormat{ imageFormat }
        , internalStorageType(internalStorageType)
        , horizontalWrapMode(wrapMode)
        , verticalWrapMode(wrapMode)
        , minFilter(filteringMode)
        , magFilter(filteringMode) { }

    TextureFormat imageFormat{ };
    TextureStorageType internalStorageType{ };

    TextureWrapMode horizontalWrapMode{ };
    TextureWrapMode verticalWrapMode{ };

    TextureFilteringMode minFilter{ };
    TextureFilteringMode magFilter{ };
};

template<int TEXTURE_TYPE>
class Texture {
public:
    Texture(glm::ivec2 size, TextureParameters parameters = TextureParameters{ }, std::vector<unsigned char> data = std::vector<unsigned char>{ }) {
        glGenTextures(1, &m_TextureHandle);
        glBindTexture(TEXTURE_TYPE, m_TextureHandle);

        glTexParameteri(TEXTURE_TYPE, GL_TEXTURE_WRAP_S, (int)parameters.horizontalWrapMode);
        glTexParameteri(TEXTURE_TYPE, GL_TEXTURE_WRAP_T, (int)parameters.verticalWrapMode);
        glTexParameteri(TEXTURE_TYPE, GL_TEXTURE_MIN_FILTER, (int)parameters.minFilter);
        glTexParameteri(TEXTURE_TYPE, GL_TEXTURE_MAG_FILTER, (int)parameters.magFilter);

        if (data.empty()) {
            glTexImage2D(TEXTURE_TYPE, 0, (int)parameters.imageFormat, size.x, size.y, 0, (int)parameters.imageFormat, (int)parameters.internalStorageType, nullptr);
        }
        else {
            glTexImage2D(TEXTURE_TYPE, 0, (int)parameters.imageFormat, size.x, size.y, 0, (int)parameters.imageFormat, (int)parameters.internalStorageType, data.data());
        }
    }

    Texture(const std::string& path, TextureParameters parameters = TextureParameters{ }, bool flip = true) {
        glGenTextures(1, &m_TextureHandle);
        glBindTexture(TEXTURE_TYPE, m_TextureHandle);

        glTexParameteri(TEXTURE_TYPE, GL_TEXTURE_WRAP_S, (int)parameters.horizontalWrapMode);
        glTexParameteri(TEXTURE_TYPE, GL_TEXTURE_WRAP_T, (int)parameters.verticalWrapMode);
        glTexParameteri(TEXTURE_TYPE, GL_TEXTURE_MIN_FILTER, (int)parameters.minFilter);
        glTexParameteri(TEXTURE_TYPE, GL_TEXTURE_MAG_FILTER, (int)parameters.magFilter);

        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(flip);
        unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 3);
        if (data) {
            glTexImage2D(TEXTURE_TYPE, 0, (int)parameters.imageFormat, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(TEXTURE_TYPE);
        }
        else {
            std::cout << "Failed to load texture: " << path << std::endl;
        }
        stbi_image_free(data);
    }

    Texture(const std::vector<std::string>& paths, TextureParameters parameters = TextureParameters{ }, bool flip = true) {
        glGenTextures(1, &m_TextureHandle);
        glBindTexture(TEXTURE_TYPE, m_TextureHandle);

        glTexParameteri(TEXTURE_TYPE, GL_TEXTURE_WRAP_S, (int)parameters.horizontalWrapMode);
        glTexParameteri(TEXTURE_TYPE, GL_TEXTURE_WRAP_T, (int)parameters.verticalWrapMode);
        glTexParameteri(TEXTURE_TYPE, GL_TEXTURE_MIN_FILTER, (int)parameters.minFilter);
        glTexParameteri(TEXTURE_TYPE, GL_TEXTURE_MAG_FILTER, (int)parameters.magFilter);

        std::vector<std::vector<unsigned char>> data;
        int width{ -1 };
        int height{ -1 };
        int channels{ -1 };

        data.resize(paths.size());

        stbi_set_flip_vertically_on_load(flip);

        size_t i = 0;
        for (auto& path : paths) {
            int w, h, c;
            unsigned char* d = stbi_load(path.c_str(), &w, &h, &c, 3);

            if ((w != width && width != -1) || (h != height && height != -1)) {
                std::cout << "Not all images: " << std::endl;

                for (auto& path : paths) {
                    std::cout << "  " << path << std::endl;
                }

                std::cout << "are the same size, cannot create texture" << std::endl;

                return;
            }

            width = w;
            height = h;
            channels = c;

            if (d) {
                data[i].resize(width * height * 3);

                std::memcpy((void*)data[i].data(), (void*)d, data[i].size());

                stbi_image_free(d);
            }
            else {
                std::cout << "Failed to load texture: " << path << std::endl;
                return;
            }

            ++i;
        }

        glTexStorage3D(TEXTURE_TYPE, 3, GL_RGB8, width, height, (int)paths.size());

        for (size_t i = 0; i < data.size(); ++i) {
            glTexSubImage3D(TEXTURE_TYPE, 0, 0, 0, (int)i, width, height, 1, GL_RGB, GL_UNSIGNED_BYTE, (void*)data[i].data());
        }

        glGenerateMipmap(TEXTURE_TYPE);
    }

    Texture(const Texture& other) = delete;
    Texture(Texture&& other) noexcept = default;
    Texture& operator=(const Texture& other) = delete;
    Texture& operator=(Texture&& other) noexcept = default;

    ~Texture() {
        glDeleteTextures(1, &m_TextureHandle);
    }

    void Bind() {
        glBindTexture(TEXTURE_TYPE, m_TextureHandle);
    }

    void Unbind() {
        glBindTexture(TEXTURE_TYPE, 0);
    }

    unsigned int Get() {
        return m_TextureHandle;
    }

    void ActivateUnit(unsigned int i) {
        glActiveTexture(GL_TEXTURE0 + i);
    }

private:
    unsigned int m_TextureHandle;
};

using Texture2D = Texture<GL_TEXTURE_2D>;
using Texture2D_Array = Texture<GL_TEXTURE_2D_ARRAY>;