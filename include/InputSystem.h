#pragma once

#include <Camera.h>
#include <InputSystemForward.h>
#include <array>

namespace oriongl::core {

class InputSystem {
    std::array<KeyState, maxKeyBufferSize> key_states;

    void updateKeyState();
    void updateCommandBuffer();

  public:
    InputSystem();
    void process();
    static void cleanup();
};
} // namespace oriongl::core
