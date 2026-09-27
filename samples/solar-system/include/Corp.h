#pragma once
#include <Model.h>
#include <array>
#include <string>
#include <vector>

namespace oriongl::samples::solar_system {
    typedef std::array<float, 3> Point;

    struct CorpData {
        std::string name;
        std::vector<std::string> defines;
        std::vector<std::string> textures;
        float radius = 1.0f;
        float rotationScaler = 1.0f;
        float translationScaler = 1.0f;
        Point pos = {0.0f, 0.0f, 0.0f};
    };

    class Corp : public graphics::Model {
        float rotationScaler;
        float translationScaler;
        glm::vec3 initialPos;
        float orbitRadius = 0.0f;
        float initialPhase = 0.0f;
        bool isStar = false;

    public:
        Corp(std::shared_ptr<graphics::Program> program,
             std::shared_ptr<graphics::Mesh> mesh,
             std::shared_ptr<graphics::Material> material,
             const CorpData &data,
             bool is_star = false);

        void draw() override;
    };
}

