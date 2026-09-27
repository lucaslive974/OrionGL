#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace oriongl::core {

struct Transform {
  glm::vec3 position{0.0f, 0.0f, 0.0f};
  glm::vec3 rotation{0.0f, 0.0f, 0.0f}; // Pitch, Yaw, Roll in degrees
  glm::vec3 scale{1.0f, 1.0f, 1.0f};

  Transform() = default;

  // Implicit constructor for seamless backwards compatibility with glm::vec3
  Transform(const glm::vec3 &pos) : position(pos) {}

  Transform(float x, float y, float z) : position(x, y, z) {}

  Transform(const glm::vec3 &pos, const glm::vec3 &rot,
            const glm::vec3 &scl = glm::vec3(1.0f))
      : position(pos), rotation(rot), scale(scl) {}

  glm::mat4 getMatrix() const {
    glm::mat4 mat = glm::mat4(1.0f);
    mat = glm::translate(mat, position);
    if (rotation.x != 0.0f)
      mat = glm::rotate(mat, glm::radians(rotation.x),
                        glm::vec3(1.0f, 0.0f, 0.0f));
    if (rotation.y != 0.0f)
      mat = glm::rotate(mat, glm::radians(rotation.y),
                        glm::vec3(0.0f, 1.0f, 0.0f));
    if (rotation.z != 0.0f)
      mat = glm::rotate(mat, glm::radians(rotation.z),
                        glm::vec3(0.0f, 0.0f, 1.0f));
    if (scale != glm::vec3(1.0f))
      mat = glm::scale(mat, scale);
    return mat;
  }
};

} // namespace oriongl::core
