//
// Created by lucas.lima on 30/11/2025.
//

#include <RenderSystem.h>
#include <glad.h>

namespace oriongl::core {
RenderSystem::RenderSystem() = default;

void RenderSystem::render(Scene &scene) {
    glClearColor(0.0F, 0.0F, 0.0F, 0.0F);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto &lights = scene.lights;
    for (auto &entitie : scene.entities) {

        auto &instances = entitie.instances;
        auto &model = entitie.model;

        auto shader = model->getModelProgram();

        shader->use();
        shader->setCamera(scene.camera);
        shader->setLights(lights);

        for (auto &instance : instances) {
            shader->setUniform4fm("model", instance.getMatrix());
            model->draw();
        };
    }
}
} // namespace oriongl::core
