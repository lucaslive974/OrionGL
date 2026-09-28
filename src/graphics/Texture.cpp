#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <array>
#include <glad.h>
#include <stdexcept>

enum : uint8_t { GL_STB_NULL_PLACEHOLDER = 0 };
// Map for the colors system of images of stb_images

namespace oriongl::graphics {
constexpr std::array<int, 4> colorSystem = {GL_STB_NULL_PLACEHOLDER, GL_STB_NULL_PLACEHOLDER, GL_RGB, GL_RGBA};

Texture::Texture(const std::string &path) : Texture(path, true) {}

Texture::Texture(const std::string &path, bool flip) {
    int width;
    int heigth;
    int nrChannels;
    stbi_set_flip_vertically_on_load(static_cast<int>(flip));
    unsigned char *data = stbi_load(path.c_str(), &width, &heigth, &nrChannels, 0);

    glGenTextures(1, &TEX);
    glBindTexture(GL_TEXTURE_2D, TEX);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    if (data != nullptr) {
        glTexImage2D(GL_TEXTURE_2D, 0, colorSystem[nrChannels - 1], width, heigth, 0, colorSystem[nrChannels - 1], GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        glDeleteTextures(1, &TEX);
        TEX = 0;
        throw std::runtime_error{"Failure loading the texture.\n" + path};
    }

    stbi_image_free(data);
}

Texture::~Texture() {
    if (TEX != 0) {
        glDeleteTextures(1, &TEX);
        TEX = 0;
    }
}

Texture::Texture(Texture &&other) noexcept : TEX(other.TEX) { other.TEX = 0; }

auto Texture::operator=(Texture &&other) noexcept -> Texture & {
    if (this != &other) {
        if (TEX != 0) {
            glDeleteTextures(1, &TEX);
        }
        TEX = other.TEX;
        other.TEX = 0;
    }
    return *this;
}

auto Texture::getTex() const -> GLuint { return TEX; }
} // namespace oriongl::graphics
