//
// Created by lucas.lima on 10/09/2025.
//

#ifndef OPENGL_LEARNING_LIGHT_H
#define OPENGL_LEARNING_LIGHT_H

#include "glm/vec3.hpp"
#include "optional"

namespace oriongl::graphics {

using LightColor = std::array<float, 3>;

struct LightScale {
    float ambient = 0.05F;
    float diffuse = 0.8F;
    float specular = 1.0F;
};

struct LightAttenuation {
    float constant = 1.0F;
    float linear = 0.0014F;
    float quadratic = 0.000007F;
};

struct Light {
    std::array<float, 3> _color{1.0F, 1.0F, 1.0F};
};

struct DirectionalLight : Light {
    glm::vec3 _direction{0.0F, 0.0F, 0.0F};

    DirectionalLight(glm::vec3 dir, LightColor color = {1.0F, 1.0F, 1.0F}) : _direction(dir), Light(color) {};
};

struct PointLight : Light {
    glm::vec3 _position;
    LightAttenuation _attenuation;

    PointLight(glm::vec3 pos, LightColor color, LightAttenuation att = LightAttenuation{}) : _position(pos), _attenuation(att), Light(color) {};
};

struct SpotLight : Light {
    glm::vec3 _position;
    glm::vec3 _direction;
    float cutOff;
};

} // namespace oriongl::graphics

namespace oriongl::core {
class Lighting {
    std::optional<graphics::DirectionalLight> directional = std::nullopt;
    std::vector<graphics::PointLight> points;
    std::vector<graphics::SpotLight> spotlights;
    graphics::LightScale lightScale;

  public:
    Lighting() = default;
    Lighting(std::vector<graphics::PointLight> &lights) : points(lights) {};
    Lighting(graphics::DirectionalLight dir_light) : directional(dir_light) {};
    Lighting(graphics::DirectionalLight dir_light, std::vector<graphics::PointLight> &lights) : directional(dir_light), points(lights) {};

    bool hasDirectional() const;
    void setDirectional(glm::vec3 light);

    size_t addPointLight(glm::vec3 position, graphics::LightColor color);
    size_t addPointLight(graphics::PointLight light);
    void removePointLight(size_t idx);

    graphics::DirectionalLight &getDirectionalLight();
    std::vector<graphics::PointLight> &getPointLights();
    graphics::LightScale &getLightScaling();
};

} // namespace oriongl::core

#endif // OPENGL_LEARNING_LIGHT_H
