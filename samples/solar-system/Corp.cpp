#include "Corp.h"
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

void Corp::update(core::Transform &transform, float totalTime) {
    if (isStar || orbitRadius < 0.001f) {
        transform.position = initialPos;
    } else {
        float angle = initialPhase + totalTime * translationScaler;
        transform.position = glm::vec3(std::cos(angle) * orbitRadius, initialPos.y, std::sin(angle) * orbitRadius);
    }

    transform.rotation.y = std::fmod(totalTime * rotationScaler * 20.0f, 360.0f);
}
} // namespace oriongl::samples::solar_system

