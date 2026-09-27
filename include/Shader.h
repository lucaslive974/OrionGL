#pragma once

#include <string>
#include <vector>

namespace oriongl::graphics {

#define VERTEX_SHADER 0x8b31
#define FRAGMENT_SHADER 0x8b30

enum ShaderType {
    VERTEX = VERTEX_SHADER,
    FRAGMENT = FRAGMENT_SHADER,
};

struct ShadersSrc {
    std::string vertex;
    std::string fragment;
};

class Shader {
  protected:
    std::string shaderSource;
    std::vector<std::string> defines;
    ShaderType shaderType;
    unsigned int ID = 0;

  public:
    Shader(ShaderType type, std::string src_raw, const std::vector<std::string> &defines = {});
    ~Shader();

    Shader(const Shader &) = delete;
    Shader &operator=(const Shader &) = delete;

    Shader(Shader &&other) noexcept;
    Shader &operator=(Shader &&other) noexcept;

    void injectDefines();
    void compileShader() const;
    [[nodiscard]] unsigned int getId() const;
    std::string getSource();
    void getErrors();
};
} // namespace oriongl::graphics
