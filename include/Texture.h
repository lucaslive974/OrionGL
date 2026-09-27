#pragma once
#include <string>

namespace oriongl::graphics {
class Texture {
    unsigned int TEX = 0;

  public:
    Texture(std::string texture);
    Texture(std::string texture, bool flip);
    ~Texture();

    Texture(const Texture &) = delete;
    Texture &operator=(const Texture &) = delete;

    Texture(Texture &&other) noexcept;
    Texture &operator=(Texture &&other) noexcept;

    unsigned int getTex() const;
};
} // namespace oriongl::graphics

