#include "Texture.h"
#include <vector>

Texture::Texture(glm::ivec2 size, Parameters parameters, std::vector<unsigned char> data) {
    glGenTextures(1, &m_TextureHandle);
    glBindTexture(GL_TEXTURE_2D, m_TextureHandle);

    // Set default parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (int)parameters.horizontalWrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (int)parameters.verticalWrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (int)parameters.minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (int)parameters.magFilter);

    if (data.empty()) {
        glTexImage2D(GL_TEXTURE_2D, 0, (int)parameters.imageFormat, size.x, size.y, 0, (int)parameters.imageFormat, (int)parameters.internalStorageType, nullptr);
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, (int)parameters.imageFormat, size.x, size.y, 0, (int)parameters.imageFormat, (int)parameters.internalStorageType, data.data());
    }
}

Texture::~Texture() {
    glDeleteTextures(1, &m_TextureHandle);
}

void Texture::Bind() {
    glBindTexture(GL_TEXTURE_2D, m_TextureHandle);
}

unsigned Texture::Get() {
    return m_TextureHandle;
}
