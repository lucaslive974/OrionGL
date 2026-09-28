#include <Light.h>

namespace oriongl::core {

auto Lighting::hasDirectional() const -> bool { return directional.has_value(); }

auto Lighting::getDirectionalLight() -> graphics::DirectionalLight & { return directional.value(); };

auto Lighting::getPointLights() -> std::vector<graphics::PointLight> & { return points; };

auto Lighting::getLightScaling() -> graphics::LightScale & { return lightScale; };

auto Lighting::addPointLight(graphics::PointLight light) -> size_t {
    points.push_back(light);
    return points.size() - 1;
};

auto Lighting::addPointLight(glm::vec3 position, graphics::LightColor color) -> size_t {
    graphics::PointLight light{position, color};
    points.push_back(light);

    return points.size() - 1;
}

void Lighting::removePointLight(size_t idx) { points.erase(points.begin() + idx); };
} // namespace oriongl::core
