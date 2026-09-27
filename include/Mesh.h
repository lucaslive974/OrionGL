#pragma once
#include <vector>

namespace oriongl::graphics {

typedef std::vector<float> vertex_array;
typedef std::vector<unsigned int> indexes_array;

struct Vertex {
    float position[3];
    float normal[3];
    float text_coords[2];
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
    void bindBuffer();

    unsigned int getVertexSize() const;
  public:
    Mesh(const vertex_array &vertexes, const indexes_array &indexes);
    ~Mesh();

    Mesh(const Mesh &) = delete;
    Mesh &operator=(const Mesh &) = delete;

    Mesh(Mesh &&other) noexcept;
    Mesh &operator=(Mesh &&other) noexcept;

    unsigned int getVAO() const;
    unsigned int getIndexSize() const;
};
} // namespace oriongl::graphics
