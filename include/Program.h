#pragma once

#include <glm/glm.hpp>
#include <memory>

#include "Camera.h"
#include "Light.h"
#include "Shader.h"

namespace oriongl::graphics {
class Program {
    glm::mat4 model = glm::mat4(1.0F);
    unsigned int ID = 0;

  public:
    Program(const std::shared_ptr<Shader> &vertex, const std::shared_ptr<Shader> &fragment);
    ~Program();

    Program(const Program &) = delete;
    Program &operator=(const Program &) = delete;

    Program(Program &&other) noexcept;
    Program &operator=(Program &&other) noexcept;

    Program &scale(glm::vec3 scaleProps);

    Program &rotate(float degree, glm::vec3 rotateProps);

    Program &translate(glm::vec3 translateProps);

    void resetModelMatrix();

    void setModelMatrix();

    void setLights(core::Lighting &lights) const;

    void setLightScale(graphics::LightScale &scaling) const;

    void setDirectionalLight(graphics::DirectionalLight &light) const;

    void setPointLights(std::vector<graphics::PointLight> &lights) const;

    void setCamera(core::Camera &camera) const;

    void setTextures() const;

    // NOLINTBEGIN
    void setUniform1I(const char name[], int value) const;

    void setUniform1UI(const char name[], int value) const;

    void setUniform1f(const char name[], float value) const;

    void setUniform3fv(const char name[], glm::vec3 vec) const;

    void setUniform4fv(const char name[], glm::vec4 vec) const;

    void setUniform4fm(const char name[], glm::mat4 mat) const;

    void setUniform1i(const char name[], int value) const;
    // NOLINTEND

    void use() const;

    [[nodiscard]] unsigned int getId() const;

    void errors();
};
} // namespace oriongl::graphics
