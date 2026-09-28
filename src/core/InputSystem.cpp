#include <InputSystem.h>
#include <array>

namespace {
using Key = oriongl::core::Key;
using KeyState = oriongl::core::KeyState;
using Command = oriongl::core::Command;
using Action = oriongl::core::Action;

oriongl::core::InputContext ctx;

struct CommandHelper {
    Key key;
    KeyState state;
    Command action;
};

std::array<CommandHelper, 7> commandHelper = {
    Key::Escape, KeyState::Pressed, {.action = Action::Quit},          Key::W,     KeyState::Pressed, {.action = Action::MoveForward},
    Key::S,      KeyState::Pressed, {.action = Action::MoveBackward},  Key::A,     KeyState::Pressed, {.action = Action::MoveLeftward},
    Key::D,      KeyState::Pressed, {.action = Action::MoveRightward}, Key::Space, KeyState::Pressed, {.action = Action::MoveUpward},
    Key::Ctrl,   KeyState::Pressed, {.action = Action::MoveDownward},
};

} // namespace

namespace oriongl::core {
InputSystem::InputSystem() { key_states.fill(KeyState::Released); };

void InputSystem::process() {
    updateKeyState();
    updateCommandBuffer();
}

void InputSystem::cleanup() { ctx.commands.clear(); }

void InputSystem::updateCommandBuffer() {
    for (auto &helper : commandHelper) {
        if (key_states[helper.key] == helper.state)
            ctx.commands.push_back(helper.action);
    };
}

void InputSystem::updateKeyState() {
    for (auto &event : ctx.events) {
        if (event.type == EventType::Keyboard) {
            Key key = KeyTranslationLayer::getKey(event.first);
            KeyState state = KeyTranslationLayer::getState(event.second);
            key_states[key] = state;
        } else {
            ctx.commands.push_back({.action = Action::LookAt, .value = {static_cast<float>(event.first), static_cast<float>(event.second)}});
        }
    }

    ctx.events.clear();
}

auto getCommands() -> const CommandBuffer & { return ctx.commands; };
auto getEvents() -> EventBuffer & { return ctx.events; };

} // namespace oriongl::core
