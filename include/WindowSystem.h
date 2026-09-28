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
    static void closeWindow();

    [[nodiscard]] float getDeltaTime() const;
    static auto getTotalTime() -> float;
};
} // namespace oriongl::core
