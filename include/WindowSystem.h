#pragma once

// clang-format off
#include <glad.h>
// clang-format on
#include <memory>

namespace oriongl::core {
class WindowSystem {
private:
  class WindowSystemImpl;
  std::unique_ptr<WindowSystemImpl> impl;

public:
  WindowSystem();
  ~WindowSystem();

  void setTitle(const char *title);
  void swapBuffers();
  void closeWindow();

  float getDeltaTime() const;
  float getTotalTime() const;
};
} // namespace oriongl::core
