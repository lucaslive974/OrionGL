#include "Shader.h"
#include <array>
#include <glad.h>
#include <sstream>
#include <utility>

namespace oriongl::graphics {
Shader::Shader(ShaderType type, std::string src_raw, const std::vector<std::string> &defines)
    : ID(glCreateShader(type)), shaderSource(std::move(std::move(src_raw))), shaderType(type), defines(defines) {
    injectDefines();
    compileShader();
    getErrors();
}

void Shader::injectDefines() {
    std::stringstream ss;
    const size_t firstBreakLine = shaderSource.find_first_of('\n');
    ss << shaderSource.substr(0, firstBreakLine);
    for (auto &define : defines) {
        ss << "\n#define " << define;
    }
    ss << "\n";
    ss << shaderSource.substr(firstBreakLine);

    shaderSource = ss.str();
}

void Shader::compileShader() const {
    const char *raw = shaderSource.c_str();
    glShaderSource(ID, 1, &raw, nullptr);
    glCompileShader(ID);
}

Shader::~Shader() {
    if (this->ID != 0) {
        glDeleteShader(this->ID);
        this->ID = 0;
    }
}

Shader::Shader(Shader &&other) noexcept
    : shaderSource(std::move(other.shaderSource)), defines(std::move(other.defines)), shaderType(other.shaderType), ID(other.ID) {
    other.ID = 0;
}

auto Shader::operator=(Shader &&other) noexcept -> Shader & {
    if (this != &other) {
        if (this->ID != 0) {
            glDeleteShader(this->ID);
        }
        shaderSource = std::move(other.shaderSource);
        defines = std::move(other.defines);
        shaderType = other.shaderType;
        ID = other.ID;
        other.ID = 0;
    }
    return *this;
}

auto Shader::getSource() -> std::string { return shaderSource; }

void Shader::getErrors() {
    int success = 0;
    std::array<char, 512> infoLog;

    glGetShaderiv(ID, GL_COMPILE_STATUS, &success);

    if (success == 0) {
        glGetShaderInfoLog(ID, 512, nullptr, infoLog.data());
        glDeleteShader(ID);
        ID = 0;
        throw std::runtime_error{infoLog.data()};
    }
}

auto Shader::getId() const -> GLuint { return ID; }

} // namespace oriongl::graphics
