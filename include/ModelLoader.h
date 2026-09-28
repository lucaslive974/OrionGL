#pragma once
#include <Material.h>
#include <Mesh.h>
#include <vector>

namespace oriongl::core {

using MeshData = std::tuple<graphics::vertex_array, graphics::indexes_array>;
using MaterialData = std::vector<std::string>;

struct ModelData {
    std::vector<MeshData> mesh_data;
    std::vector<MaterialData> material_data;
};

class ModelLoader {
  public:
    static ModelData loadFromFile(const std::string &src);
};

} // namespace oriongl::core
