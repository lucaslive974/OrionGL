#pragma once

#include <array>
#include <cstdint>
#include <vector>

constexpr const int maxKeyBufferSize = 516;

namespace oriongl::core {

enum Key : uint16_t {
    Unknown,
    // Moment
    W,
    A,
    S,
    D,

    // Modifiers
    Ctrl,
    Alt,
    Space,
    Escape,
};

enum KeyState : uint8_t {
    Pressed,
    Released,
    Repeat,
};

class KeyTranslationLayer {
  public:
    static Key getKey(int key_code);
    static KeyState getState(int state_code);
};

enum EventType : uint8_t { Mouse, Keyboard };

struct Event {
    EventType type;
    double first;
    double second;
};

using EventBuffer = std::vector<Event>;

enum Action : uint8_t {
    MoveForward,
    MoveBackward,
    MoveLeftward,
    MoveRightward,
    MoveUpward,
    MoveDownward,

    LookAt,

    Quit,
};

struct Command {
    Action action;
    std::array<double, 2> value;
};

using CommandBuffer = std::vector<Command>;
using DeferredBuffer = CommandBuffer;

struct InputContext {
    EventBuffer events;
    CommandBuffer commands;
    DeferredBuffer deferred;
};

const CommandBuffer &getCommands();
EventBuffer &getEvents();

} // namespace oriongl::core
