#include "Program.h"
#include "Shader.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <algorithm>

#include <glad.h>

namespace {
const size_t MAX_NUMBER_POINT_LIGHTS = 8;
}

namespace oriongl::graphics {
Program::Program(std::shared_ptr<Shader> vertex, std::shared_ptr<Shader> fragment) {
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
    other.model = glm::mat4(1.0f);
}

Program &Program::operator=(Program &&other) noexcept {
    if (this != &other) {
        if (ID != 0) {
            glDeleteProgram(ID);
        }
        model = other.model;
        ID = other.ID;
        other.ID = 0;
        other.model = glm::mat4(1.0f);
    }
    return *this;
}

void Program::errors() {
    int success = 0;
    char infoLog[512];
    glGetProgramiv(ID, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        glDeleteProgram(ID);
        ID = 0;
        throw std::runtime_error{infoLog};
    }
}

void Program::use() { glUseProgram(ID); }

unsigned int Program::getId() { return ID; }

void Program::resetModelMatrix() { model = glm::mat4(1.0f); }

void Program::setModelMatrix() { setUniform4fm("model", model); }

Program &Program::scale(glm::vec3 scaleProps) {
    model = glm::scale(model, scaleProps);
    return *this;
};

Program &Program::rotate(float degree, glm::vec3 rotateProps) {
    model = glm::rotate(model, glm::radians(degree), rotateProps);
    return *this;
};

Program &Program::translate(glm::vec3 translateProps) {
    model = glm::translate(model, translateProps);
    return *this;
}

void Program::setCamera(core::Camera &camera) {
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
void Program::setLights(core::Lighting &lights) {
    setLightScale(lights.getLightScaling());

    bool _res = lights.hasDirectional();
    setUniform1i("hasDirectional", _res);

    if (_res)
        setDirectionalLight(lights.getDirectionalLight());

    setPointLights(lights.getPointLights());
}

void Program::setLightScale(graphics::LightScale &scaling) {
    setUniform1f("lightScaling.ambient", scaling.ambient);
    setUniform1f("lightScaling.diffuse", scaling.diffuse);
    setUniform1f("lightScaling.specular", scaling.specular);
}

void Program::setDirectionalLight(graphics::DirectionalLight &light) {
    auto &_direction = light._direction;
    auto &_color = light._color;

    setUniform3fv("directional.position", glm::vec3(0.0f));
    setUniform3fv("directional.direction", _direction);
    setUniform3fv("directional.color", glm::vec3(_color[0], _color[1], _color[2]));
    setUniform1f("directional.cutOff", 0.0f);
}

void Program::setPointLights(std::vector<graphics::PointLight> &lights) {
    size_t n_lights = std::min(lights.size(), MAX_NUMBER_POINT_LIGHTS);

    setUniform1i("nPointLights", n_lights);

    for (size_t i = 0; i < n_lights; i++) {
        auto &position = lights[i]._position;
        auto &color = lights[i]._color;
        auto &attenuation = lights[i]._attenuation;

        std::string label = "pointLights[" + std::to_string(i) + "]";

        setUniform3fv((label + ".position").c_str(), position);
        setUniform3fv((label + ".direction").c_str(), glm::vec3(0.0f));
        setUniform3fv((label + ".color").c_str(), glm::vec3(color[0], color[1], color[2]));
        setUniform1f((label + ".cutOff").c_str(), 0.0f);

        setUniform1f((label + ".attenuation.constant").c_str(), attenuation.constant);
        setUniform1f((label + ".attenuation.linear").c_str(), attenuation.linear);
        setUniform1f((label + ".attenuation.quadratic").c_str(), attenuation.quadratic);
    }
}

void Program::setTextures() {
    // Texture
    setUniform1i("material.diffuse", 0);
    setUniform1i("material.specular", 1);
    setUniform1i("material.emission", 2);
    setUniform1f("material.shininess", 1.0f);
}

void Program::setUniform1I(const char name[], GLint value) const { glUniform1i(glGetUniformLocation(ID, name), value); }

void Program::setUniform1f(const char name[], GLfloat value) const { glUniform1f(glGetUniformLocation(ID, name), value); }

void Program::setUniform3fv(const char name[], glm::vec3 vec) {
    glUniform3fv(glGetUniformLocation(ID, name), 1, glm::value_ptr(vec));
}

void Program::setUniform4fv(const char name[], glm::vec4 vec) {
    glUniform4fv(glGetUniformLocation(ID, name), 1, glm::value_ptr(vec));
}

void Program::setUniform4fm(const char name[], glm::mat4 mat) {
    glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, glm::value_ptr(mat));
}

void Program::setUniform1i(const char name[], GLint value) { glUniform1i(glGetUniformLocation(ID, name), value); }
} // namespace oriongl::graphics
