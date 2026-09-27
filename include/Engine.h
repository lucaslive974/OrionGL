#pragma once

// clang-format off
#include <WindowSystem.h>
// clang-format on

#include <InputSystem.h>
#include <RenderSystem.h>
#include <ResourceSystem.h>
#include <Scene.h>
#include <functional>

namespace oriongl {

enum EngineStatus {
  ENGINE_RUNNING,
  ENGINE_CLOSING,
  ENGINE_STOPPED,
};

using UpdateCallback =
    std::function<void(core::Scene &scene, float dt, float totalTime)>;

class Engine {
private:
  core::WindowSystem windowSystem;
  core::ResourceSystem resourceSystem;
  core::RenderSystem renderSystem;
  core::InputSystem inputSystem;

  core::Scene scene;

  EngineStatus status = ENGINE_STOPPED;
  UpdateCallback onUpdate = nullptr;

  void processCommands();

public:
  Engine() = default;
  void run();
  void setScene(core::Scene &sc);
  void setUpdateCallback(UpdateCallback cb);
  core::WindowSystem &getWindowSystem();
};

} // namespace oriongl
