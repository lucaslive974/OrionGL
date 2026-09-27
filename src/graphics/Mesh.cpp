#include "Mesh.h"
#include <cassert>
#include <glad.h>

namespace oriongl::graphics {
Mesh::Mesh(const vertex_array &vertexes, const indexes_array &indexes) {
    genVertexArrayBuffer();
    genVertexBufferObject(vertexes);
    genElementBufferObject(indexes);
    bindBuffer();
}

Mesh::~Mesh() {
    if (EBO) {
        glDeleteBuffers(1, &EBO);
        EBO = 0;
    }
    if (VBO) {
        glDeleteBuffers(1, &VBO);
        VBO = 0;
    }
    if (VAO) {
        glDeleteVertexArrays(1, &VAO);
        VAO = 0;
    }
}

Mesh::Mesh(Mesh &&other) noexcept
    : vertexesSize(other.vertexesSize),
      indexesSize(other.indexesSize),
      VAO(other.VAO),
      VBO(other.VBO),
      EBO(other.EBO) {
    other.vertexesSize = 0;
    other.indexesSize = 0;
    other.VAO = 0;
    other.VBO = 0;
    other.EBO = 0;
}

Mesh &Mesh::operator=(Mesh &&other) noexcept {
    if (this != &other) {
        if (EBO) glDeleteBuffers(1, &EBO);
        if (VBO) glDeleteBuffers(1, &VBO);
        if (VAO) glDeleteVertexArrays(1, &VAO);

        vertexesSize = other.vertexesSize;
        indexesSize = other.indexesSize;
        VAO = other.VAO;
        VBO = other.VBO;
        EBO = other.EBO;

        other.vertexesSize = 0;
        other.indexesSize = 0;
        other.VAO = 0;
        other.VBO = 0;
        other.EBO = 0;
    }
    return *this;
}


void Mesh::genVertexBufferObject(const vertex_array &vertexes) {
    vertexesSize = vertexes.size();

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexesSize * sizeof(float), vertexes.data(), GL_STATIC_DRAW);
}

void Mesh::genElementBufferObject(const std::vector<unsigned int> &indexes) {
    indexesSize = indexes.size();

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexesSize * sizeof(unsigned int), indexes.data(), GL_STATIC_DRAW);
}

void Mesh::genVertexArrayBuffer() {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
}

void Mesh::bindBuffer() {
    static_assert(sizeof(Vertex) == 32);
    static_assert(sizeof(Vertex::position) == 12);
    static_assert(sizeof(Vertex::normal) == 12);
    static_assert(sizeof(Vertex::text_coords) == 8);

    glVertexAttribPointer(0, sizeof(Vertex::position) / sizeof(float), GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, sizeof(Vertex::normal) / sizeof(float), GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, sizeof(Vertex::text_coords) / sizeof(float), GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)offsetof(Vertex, text_coords));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

unsigned int Mesh::getVAO() const { return VAO; }

unsigned int Mesh::getVertexSize() const { return vertexesSize; }

unsigned int Mesh::getIndexSize() const { return indexesSize; }
} // namespace oriongl::graphics
