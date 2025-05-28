#pragma once
#include <vector>

#include <gl/glew.h>
#include <GLFW/glfw3.h>

#include <glm/vec2.hpp>

class Texture {
public:
    enum class Format {
        R = GL_RED,
        RGB = GL_RGB,
        RGBA = GL_RGBA
    };

    enum class StorageType {
        UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
        FLOAT = GL_FLOAT
    };

    enum class WrapMode {
        REPEAT = GL_REPEAT
    };

    enum class FilteringMode {
        LINEAR = GL_LINEAR,
        NEAREST = GL_NEAREST
    };

    struct Parameters {
        Parameters(
            Format imageFormat = Format::RGBA, 
            StorageType internalStorageType = StorageType::UNSIGNED_BYTE, 
            WrapMode wrapMode = WrapMode::REPEAT, 
            FilteringMode filteringMode = FilteringMode::LINEAR
        )
            : imageFormat{ imageFormat }
            , internalStorageType(internalStorageType)
            , horizontalWrapMode(wrapMode)
            , verticalWrapMode(wrapMode)
            , minFilter(filteringMode)
            , magFilter(filteringMode) { }

        Format imageFormat{ };
        StorageType internalStorageType{ };

        WrapMode horizontalWrapMode{ };
        WrapMode verticalWrapMode{ };

        FilteringMode minFilter{ };
        FilteringMode magFilter{ };
    };

    Texture(glm::ivec2 size, Parameters parameters = Parameters{ }, std::vector<unsigned char> data = std::vector<unsigned char>{ });
    Texture(const Texture& other) = delete;
    Texture(Texture&& other) noexcept = default;
    Texture& operator=(const Texture& other) = delete;
    Texture& operator=(Texture&& other) noexcept = default;
    ~Texture();

    void Bind();

    unsigned int Get();

private:
    unsigned int m_TextureHandle;
};