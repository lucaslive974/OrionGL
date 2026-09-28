#pragma once
#include <array>
#include <vector>

namespace oriongl::graphics {

using vertex_array = std::vector<float>;
using indexes_array = std::vector<unsigned int>;

struct Vertex {
    std::array<float, 3> position;
    std::array<float, 3> normal;
    std::array<float, 2> text_coords;
};

class Mesh {
    unsigned int vertexesSize = 0;
    unsigned int indexesSize = 0;
    unsigned int VAO = 0;
    unsigned int VBO = 0;
    unsigned int EBO = 0;

    void genVertexArrayBuffer();
    void genVertexBufferObject(const vertex_array &vertexes);
    void genElementBufferObject(const indexes_array &indexes);
    static void bindBuffer();

    [[nodiscard]] unsigned int getVertexSize() const;

  public:
    Mesh(const vertex_array &vertexes, const indexes_array &indexes);
    ~Mesh();

    Mesh(const Mesh &) = delete;
    Mesh &operator=(const Mesh &) = delete;

    Mesh(Mesh &&other) noexcept;
    Mesh &operator=(Mesh &&other) noexcept;

    [[nodiscard]] unsigned int getVAO() const;
    [[nodiscard]] unsigned int getIndexSize() const;
};
} // namespace oriongl::graphics
