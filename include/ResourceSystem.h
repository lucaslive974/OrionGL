#pragma once
#include <Manager.h>

#include <Material.h>
#include <Mesh.h>
#include <Model.h>
#include <Program.h>
#include <Shader.h>
#include <Texture.h>

namespace oriongl::core {

class ResourceSystem {
    storage::Manager<graphics::Shader> shader_manager;
    storage::Manager<graphics::Program> program_manager;
    storage::Manager<graphics::Mesh> mesh_manager;
    storage::Manager<graphics::Texture> texture_manager;
    storage::Manager<graphics::Material> material_manager;
    storage::Manager<graphics::Model> model_manager;

    static constexpr std::pair<graphics::vertex_array, graphics::indexes_array> generateSphereMeshData(float radius);
    static std::pair<graphics::vertex_array, graphics::indexes_array> generateCubeMeshData(float side_size);

  public:
    ResourceSystem() = default;
    std::shared_ptr<graphics::Mesh> createCubeMesh(float side_size);
    std::shared_ptr<graphics::Mesh> createSphereMesh(float radius);
    std::shared_ptr<graphics::Mesh> createMesh(std::string key, graphics::vertex_array vertexes, graphics::indexes_array indexes);
    std::shared_ptr<graphics::Shader> createShaderStage(graphics::ShaderType type, std::string src, std::vector<std::string> defines);
    std::shared_ptr<graphics::Program> createShader(const std::string &vertex_src, const std::string &frag_src,
                                                    const std::vector<std::string> &defines = {});
    std::shared_ptr<graphics::Material> createMaterial(const std::vector<std::string> &textures);
    std::shared_ptr<graphics::Texture> createTexture(std::string src);
    std::shared_ptr<graphics::Model> createModel(std::string model_id, std::shared_ptr<graphics::Program> program,
                                                 std::shared_ptr<graphics::Mesh> mesh, std::shared_ptr<graphics::Material> material = nullptr);
    std::shared_ptr<graphics::Model> createModel(const std::string &model_id, std::shared_ptr<graphics::Program> program, const std::string &src);
};
} // namespace oriongl::core
