//
// Created by lucas.lima on 10/09/2025.
//

#ifndef OPENGL_LEARNING_MATERIAL_H
#define OPENGL_LEARNING_MATERIAL_H

#include "Program.h"
#include "Texture.h"

#include <array>
#include <memory>
#include <vector>

namespace oriongl::graphics {

using MaterialColor = std::array<float, 3>;

class Material {
    std::vector<std::shared_ptr<Texture>> textures;
    MaterialColor _color = {1.0F, 1.0F, 1.0F};

  public:
    Material();
    void loadTexture(std::shared_ptr<Texture> texture);
    void bindMaterial(const std::shared_ptr<Program> &prg);
    void setColor(MaterialColor color);
};
} // namespace oriongl::graphics

#endif // OPENGL_LEARNING_MATERIAL_H
