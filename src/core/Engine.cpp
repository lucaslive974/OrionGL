#include <Engine.h>

namespace oriongl {

void Engine::run() {
  status = ENGINE_RUNNING;
  while (status == ENGINE_RUNNING) {
    float dt = windowSystem.getDeltaTime();
    float totalTime = windowSystem.getTotalTime();

    inputSystem.process();
    scene.camera.processCommands(dt);

    if (onUpdate) {
      onUpdate(scene, dt, totalTime);
    }

    renderSystem.render(scene);
    windowSystem.swapBuffers();

    processCommands();
    inputSystem.cleanup();
  }

  windowSystem.closeWindow();
}

void Engine::processCommands() {
  auto command_buffer = core::getCommands();

  for (auto &command : command_buffer) {
    if (command.action == core::Action::Quit) {
      status = ENGINE_CLOSING;
      break;
    }
  }
}

void Engine::setScene(core::Scene &sc) { scene = sc; }

void Engine::setUpdateCallback(UpdateCallback cb) { onUpdate = std::move(cb); }

core::WindowSystem &Engine::getWindowSystem() { return windowSystem; }

} // namespace oriongl
