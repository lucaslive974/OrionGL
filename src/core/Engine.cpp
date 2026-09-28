#include <Engine.h>

namespace oriongl {

void Engine::run() {
    status = ENGINE_RUNNING;
    while (status == ENGINE_RUNNING) {
        float dt = windowSystem.getDeltaTime();
        float totalTime = oriongl::core::WindowSystem::getTotalTime();

        inputSystem.process();
        scene.camera.processCommands(dt);

        if (onUpdate) {
            onUpdate(scene, dt, totalTime);
        }

        oriongl::core::RenderSystem::render(scene);
        windowSystem.swapBuffers();

        processCommands();
        oriongl::core::InputSystem::cleanup();
    }

    oriongl::core::WindowSystem::closeWindow();
}

void Engine::processCommands() {
    auto commandBuffer = core::getCommands();

    for (auto &command : commandBuffer) {
        if (command.action == core::Action::Quit) {
            status = ENGINE_CLOSING;
            break;
        }
    }
}

void Engine::setScene(core::Scene &sc) { scene = sc; }

void Engine::setUpdateCallback(UpdateCallback cb) { onUpdate = std::move(cb); }

auto Engine::getWindowSystem() -> core::WindowSystem & { return windowSystem; }

} // namespace oriongl
