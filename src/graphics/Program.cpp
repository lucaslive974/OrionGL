#include "Program.h"
#include "Shader.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <algorithm>
#include <array>

#include <glad.h>

namespace {
const size_t maxNumberPointLights = 8;
}

namespace oriongl::graphics {
Program::Program(const std::shared_ptr<Shader> &vertex, const std::shared_ptr<Shader> &fragment) { // NOLINT
    ID = glCreateProgram();
    glAttachShader(ID, vertex->getId());
    glAttachShader(ID, fragment->getId());
    glLinkProgram(ID);

    errors();
}

Program::~Program() {
    if (ID != 0) {
        glDeleteProgram(ID);
        ID = 0;
    }
}

Program::Program(Program &&other) noexcept : model(other.model), ID(other.ID) {
    other.ID = 0;
    other.model = glm::mat4(1.0F);
}

auto Program::operator=(Program &&other) noexcept -> Program & {
    if (this != &other) {
        if (ID != 0) {
            glDeleteProgram(ID);
        }
        model = other.model;
        ID = other.ID;
        other.ID = 0;
        other.model = glm::mat4(1.0F);
    }
    return *this;
}

void Program::errors() {
    int success = 0;
    std::array<char, 512> infoLog;
    glGetProgramiv(ID, GL_LINK_STATUS, &success);

    if (success == 0) {
        glGetProgramInfoLog(ID, 512, nullptr, infoLog.data());
        glDeleteProgram(ID);
        ID = 0;
        throw std::runtime_error{infoLog.data()};
    }
}

void Program::use() const { glUseProgram(ID); }

auto Program::getId() const -> unsigned int { return ID; }

void Program::resetModelMatrix() { model = glm::mat4(1.0F); }

void Program::setModelMatrix() { setUniform4fm("model", model); }

auto Program::scale(glm::vec3 scaleProps) -> Program & {
    model = glm::scale(model, scaleProps);
    return *this;
};

auto Program::rotate(float degree, glm::vec3 rotateProps) -> Program & {
    model = glm::rotate(model, glm::radians(degree), rotateProps);
    return *this;
};

auto Program::translate(glm::vec3 translateProps) -> Program & {
    model = glm::translate(model, translateProps);
    return *this;
}

void Program::setCamera(core::Camera &camera) const {
    setUniform4fm("view", camera.getView());
    setUniform3fv("viewPos", camera.getViewPosition());
    setUniform4fm("projection", camera.getPerspective());
}

// Shader light structure
// struct LightScaling {
//  float ambient;
//  float diffuse;
//  float specular;
//}
// struct Light {
//   vec4 position; position.w set to 0.0f when directional.
//   vec4 direction; direction.w set to 0.0f when point light.
//   vec3 color; default to white.
//   float cutOff; 0.0f when not spotlight.
// }
void Program::setLights(core::Lighting &lights) const {
    setLightScale(lights.getLightScaling());

    bool res = lights.hasDirectional();
    setUniform1i("hasDirectional", static_cast<int>(res));

    if (res)
        setDirectionalLight(lights.getDirectionalLight());

    setPointLights(lights.getPointLights());
}

void Program::setLightScale(graphics::LightScale &scaling) const {
    setUniform1f("lightScaling.ambient", scaling.ambient);
    setUniform1f("lightScaling.diffuse", scaling.diffuse);
    setUniform1f("lightScaling.specular", scaling.specular);
}

void Program::setDirectionalLight(graphics::DirectionalLight &light) const {
    auto &direction = light._direction;
    auto &color = light._color;

    setUniform3fv("directional.position", glm::vec3(0.0F));
    setUniform3fv("directional.direction", direction);
    setUniform3fv("directional.color", glm::vec3(color[0], color[1], color[2]));
    setUniform1f("directional.cutOff", 0.0F);
}

void Program::setPointLights(std::vector<graphics::PointLight> &lights) const {
    size_t nLights = std::min(lights.size(), maxNumberPointLights);

    setUniform1i("nPointLights", nLights);

    for (size_t i = 0; i < nLights; i++) {
        auto &position = lights[i]._position;
        auto &color = lights[i]._color;
        auto &attenuation = lights[i]._attenuation;

        std::string label = "pointLights[" + std::to_string(i) + "]";

        setUniform3fv((label + ".position").c_str(), position);
        setUniform3fv((label + ".direction").c_str(), glm::vec3(0.0F));
        setUniform3fv((label + ".color").c_str(), glm::vec3(color[0], color[1], color[2]));
        setUniform1f((label + ".cutOff").c_str(), 0.0F);

        setUniform1f((label + ".attenuation.constant").c_str(), attenuation.constant);
        setUniform1f((label + ".attenuation.linear").c_str(), attenuation.linear);
        setUniform1f((label + ".attenuation.quadratic").c_str(), attenuation.quadratic);
    }
}

void Program::setTextures() const {
    // Texture
    setUniform1i("material.diffuse", 0);
    setUniform1i("material.specular", 1);
    setUniform1i("material.emission", 2);
    setUniform1f("material.shininess", 1.0F);
}

void Program::setUniform1I(const char name[], GLint value) const { glUniform1i(glGetUniformLocation(ID, name), value); }

void Program::setUniform1f(const char name[], GLfloat value) const { glUniform1f(glGetUniformLocation(ID, name), value); }

void Program::setUniform3fv(const char name[], glm::vec3 vec) const { glUniform3fv(glGetUniformLocation(ID, name), 1, glm::value_ptr(vec)); }

void Program::setUniform4fv(const char name[], glm::vec4 vec) const { glUniform4fv(glGetUniformLocation(ID, name), 1, glm::value_ptr(vec)); }

void Program::setUniform4fm(const char name[], glm::mat4 mat) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, glm::value_ptr(mat));
}

void Program::setUniform1i(const char name[], GLint value) const { glUniform1i(glGetUniformLocation(ID, name), value); }
} // namespace oriongl::graphics
