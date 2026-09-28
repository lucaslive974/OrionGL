#pragma once
#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include "Model.h"
#include "Transform.h"

namespace oriongl::core {

struct Entity {
    std::shared_ptr<graphics::Model> model;
    std::vector<Transform> instances;
};

} // namespace oriongl::core
