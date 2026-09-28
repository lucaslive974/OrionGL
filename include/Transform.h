#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace oriongl::core {

struct Transform {
    glm::vec3 position{0.0F, 0.0F, 0.0F};
    glm::vec3 rotation{0.0F, 0.0F, 0.0F}; // Pitch, Yaw, Roll in degrees
    glm::vec3 scale{1.0F, 1.0F, 1.0F};

    Transform() = default;

    // Implicit constructor for seamless backwards compatibility with glm::vec3
    Transform(const glm::vec3 &pos) : position(pos) {}

    Transform(float x, float y, float z) : position(x, y, z) {}

    Transform(const glm::vec3 &pos, const glm::vec3 &rot, const glm::vec3 &scl = glm::vec3(1.0F)) // NOLINT
        : position(pos), rotation(rot), scale(scl) {}

    [[nodiscard]] glm::mat4 getMatrix() const {
        auto mat = glm::mat4(1.0F);
        mat = glm::translate(mat, position);
        if (rotation.x != 0.0F)
            mat = glm::rotate(mat, glm::radians(rotation.x), glm::vec3(1.0F, 0.0F, 0.0F));
        if (rotation.y != 0.0F)
            mat = glm::rotate(mat, glm::radians(rotation.y), glm::vec3(0.0F, 1.0F, 0.0F));
        if (rotation.z != 0.0F)
            mat = glm::rotate(mat, glm::radians(rotation.z), glm::vec3(0.0F, 0.0F, 1.0F));
        if (scale != glm::vec3(1.0F))
            mat = glm::scale(mat, scale);
        return mat;
    }
};

} // namespace oriongl::core
