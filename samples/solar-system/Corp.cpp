#include "Corp.h"
#include <GLFW/glfw3.h>
#include <cmath>

namespace oriongl::samples::solar_system {
Corp::Corp(std::shared_ptr<graphics::Program> program,
           std::shared_ptr<graphics::Mesh> mesh,
           std::shared_ptr<graphics::Material> material,
           const CorpData &data,
           bool is_star)
    : Model(program),
      rotationScaler(data.rotationScaler),
      translationScaler(data.translationScaler),
      initialPos(data.pos[0], data.pos[1], data.pos[2]),
      isStar(is_star) {
    loadData(std::move(mesh), std::move(material));

    orbitRadius = std::sqrt(initialPos.x * initialPos.x + initialPos.z * initialPos.z);
    if (orbitRadius > 0.001f) {
        initialPhase = std::atan2(initialPos.z, initialPos.x);
    } else {
        initialPhase = 0.0f;
    }
}

void Corp::draw() {
    float time = static_cast<float>(glfwGetTime());
    glm::vec3 currentPos;

    if (isStar || orbitRadius < 0.001f) {
        currentPos = initialPos;
    } else {
        float angle = initialPhase + time * translationScaler;
        currentPos = glm::vec3(std::cos(angle) * orbitRadius, initialPos.y, std::sin(angle) * orbitRadius);
    }

    float rotAngle = glm::mod(time * rotationScaler * 20.0f, 360.0f);

    program->resetModelMatrix();
    program->translate(currentPos);
    program->rotate(rotAngle, glm::vec3(0.0f, 1.0f, 0.0f));
    program->setModelMatrix();

    Model::draw();
}
} // namespace oriongl::samples::solar_system

