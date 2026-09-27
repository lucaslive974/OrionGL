#include "Shader.h"
#include "Utils.h"
#include <format>
#include <glad.h>
#include <sstream>

namespace oriongl::graphics {
Shader::Shader(ShaderType type, std::string src_raw, const std::vector<std::string> &defines)
    : ID(glCreateShader(type)), shaderSource(src_raw), shaderType(type), defines(defines) {
    injectDefines();
    compileShader();
    getErrors();
}

void Shader::injectDefines() {
    std::stringstream ss;
    const size_t first_break_line = shaderSource.find_first_of("\n");
    ss << shaderSource.substr(0, first_break_line);
    for (auto &define : defines) {
        ss << "\n#define " << define;
    }
    ss << "\n";
    ss << shaderSource.substr(first_break_line);

    shaderSource = ss.str();
}

void Shader::compileShader() const {
    const char *raw = shaderSource.c_str();
    glShaderSource(ID, 1, &raw, 0);
    glCompileShader(ID);
}

Shader::~Shader() {
    if (this->ID != 0) {
        glDeleteShader(this->ID);
        this->ID = 0;
    }
}

Shader::Shader(Shader &&other) noexcept
    : shaderSource(std::move(other.shaderSource)),
      defines(std::move(other.defines)),
      shaderType(other.shaderType),
      ID(other.ID) {
    other.ID = 0;
}

Shader &Shader::operator=(Shader &&other) noexcept {
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

std::string Shader::getSource() { return shaderSource; }

void Shader::getErrors() {
    int success = 0;
    char infoLog[512];

    glGetShaderiv(ID, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(ID, 512, NULL, infoLog);
        glDeleteShader(ID);
        ID = 0;
        throw std::runtime_error{infoLog};
    }
}

GLuint Shader::getId() const { return ID; }

} // namespace oriongl::graphics
