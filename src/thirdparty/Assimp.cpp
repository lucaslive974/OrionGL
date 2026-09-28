#include <ModelLoader.h>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <cassert>
#include <filesystem>
#include <span>
#include <string>

namespace oriongl::core {

class ModelLoaderImpl {
    ModelData data;
    const aiScene *scene = nullptr;
    const std::string model_path;
    std::filesystem::path model_root_path;

  public:
    ModelLoaderImpl(const std::string &model_path) : model_path(model_path) { model_root_path = std::filesystem::path(model_path).parent_path(); };
    auto process() -> ModelData;
    void processNode(aiNode *node);
    void processMesh(unsigned int meshId);
    static auto processVertex(aiMesh *mesh) -> graphics::vertex_array;
    static auto processIndexes(aiMesh *mesh) -> graphics::indexes_array;
    auto processMaterial(unsigned int materialId) -> MaterialData;
    inline auto getRelativeModelTexturePath(const aiString &texture_path) -> std::string;
};

auto ModelLoaderImpl::process() -> ModelData {
    Assimp::Importer importer;

    scene = importer.ReadFile(model_path, aiProcess_Triangulate | aiProcess_PreTransformVertices);

    if ((scene == nullptr) || ((scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) != 0U) || (scene->mRootNode == nullptr)) {
        std::string errorMsg{importer.GetErrorString()};
        throw std::runtime_error("ERROR::ASSIMP::" + errorMsg);
    }

    processNode(scene->mRootNode);
    return data;
};

void ModelLoaderImpl::processNode(aiNode *node) {
    for (size_t it = 0; it < node->mNumMeshes; it++) {
        processMesh(node->mMeshes[it]);
    }

    for (size_t it = 0; it < node->mNumChildren; it++) {
        processNode(node->mChildren[it]);
    }
};

void ModelLoaderImpl::processMesh(unsigned int meshId) {
    aiMesh *mesh = scene->mMeshes[meshId];

    auto vertexes = processVertex(mesh);
    auto indexes = processIndexes(mesh);
    auto material = processMaterial(mesh->mMaterialIndex);

    data.mesh_data.emplace_back(vertexes, indexes);
    data.material_data.push_back(material);
};

auto ModelLoaderImpl::processVertex(aiMesh *mesh) -> graphics::vertex_array {
    graphics::vertex_array vertexes;

    bool hasNormals = mesh->HasNormals();
    bool hasTextCoords = mesh->HasTextureCoords(0);

    for (size_t i = 0; i < mesh->mNumVertices; i++) {
        aiVector3D &aiVertexes = mesh->mVertices[i];
        aiVector3D &aiNormals = mesh->mNormals[i];
        aiVector3D &aiTextCoords = mesh->mTextureCoords[0][i];

        vertexes.insert(vertexes.end(), {aiVertexes.x, aiVertexes.y, aiVertexes.z});

        if (hasNormals)
            vertexes.insert(vertexes.end(), {aiNormals.x, aiNormals.y, aiNormals.z});
        else
            vertexes.insert(vertexes.end(), 3, 0.0F);

        if (hasTextCoords)
            vertexes.insert(vertexes.end(), {aiTextCoords.x, aiTextCoords.y});
        else
            vertexes.insert(vertexes.end(), 2, 0.0F);
    }

    return vertexes;
}

auto ModelLoaderImpl::processIndexes(aiMesh *mesh) -> graphics::indexes_array {
    graphics::indexes_array indexes;

    for (size_t i = 0; i < mesh->mNumFaces; i++) {
        aiFace &face = mesh->mFaces[i];
        auto indexesSpan = std::span(face.mIndices, face.mNumIndices);
        indexes.insert(indexes.end(), indexesSpan.begin(), indexesSpan.end());
    }

    return indexes;
}

auto ModelLoaderImpl::processMaterial(unsigned int materialId) -> MaterialData {
    std::vector<std::string> textures;

    aiMaterial *material = scene->mMaterials[materialId];
    unsigned int diffuseCnt = material->GetTextureCount(aiTextureType_DIFFUSE);
    unsigned int specularCnt = material->GetTextureCount(aiTextureType_SPECULAR);
    unsigned int emissiveCnt = material->GetTextureCount(aiTextureType_EMISSIVE);

    if (diffuseCnt != 0U) {
        aiString path;
        material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
        if (!path.Empty())
            textures.push_back(getRelativeModelTexturePath(path));
    }

    if (specularCnt != 0U) {
        aiString path;
        material->GetTexture(aiTextureType_SPECULAR, 0, &path);
        if (!path.Empty())
            textures.push_back(getRelativeModelTexturePath(path));
    }

    if (emissiveCnt != 0U) {
        aiString path;
        material->GetTexture(aiTextureType_EMISSIVE, 0, &path);
        if (!path.Empty())
            textures.push_back(getRelativeModelTexturePath(path));
    }

    return textures;
};

inline auto ModelLoaderImpl::getRelativeModelTexturePath(const aiString &texture_path) -> std::string { return model_root_path / texture_path.data; };

auto ModelLoader::loadFromFile(const std::string &src) -> ModelData {
    ModelLoaderImpl modelImpl{src};
    return modelImpl.process();
};

} // namespace oriongl::core
