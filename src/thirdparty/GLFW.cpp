// clang-format off
#include <WindowSystem.h>
// clang-format on
#include <GLFW/glfw3.h>
#include <InputSystemForward.h>
#include <array>

namespace {

using Key = oriongl::core::Key;
using KeyState = oriongl::core::KeyState;

constexpr auto key_buffer_lut = [] {
  std::array<Key, MAX_KEY_BUFFER_SIZE> lut{};
  lut.fill(Key::Unknown);

  lut[GLFW_KEY_W] = Key::W;
  lut[GLFW_KEY_A] = Key::A;
  lut[GLFW_KEY_S] = Key::S;
  lut[GLFW_KEY_D] = Key::D;
  lut[GLFW_KEY_LEFT_CONTROL] = Key::Ctrl;
  lut[GLFW_KEY_SPACE] = Key::Space;
  lut[GLFW_KEY_ESCAPE] = Key::Escape;

  return lut;
}();

constexpr auto state_buffer_lut = [] {
  std::array<KeyState, 4> lut{};

  lut[GLFW_RELEASE] = KeyState::Released;
  lut[GLFW_PRESS] = KeyState::Pressed;
  lut[GLFW_REPEAT] = KeyState::Repeat;

  return lut;
}();

} // namespace

namespace oriongl::core {

Key KeyTranslationLayer::getKey(int key_code) {
  if (key_code > 0 && key_code < MAX_KEY_BUFFER_SIZE)
    return key_buffer_lut[key_code];
  return Key::Unknown;
}

KeyState KeyTranslationLayer::getState(int state_code) {
  if (state_code >= 0 && state_code < 4)
    return state_buffer_lut[state_code];
  return KeyState::Released;
}

class WindowSystem::WindowSystemImpl {
public:
  WindowSystemImpl() {
    glfwConfiguration();
    windowInicialization();
    gladConfiguration();
  };

  GLFWwindow *window = nullptr;
  GLFWmonitor *monitor = nullptr;
  const GLFWvidmode *vidmode = nullptr;

  int width = 1280;
  float deltaTime = 0.016f;
  float lastFrame = 0;

  // Glad configuration;
  void gladConfiguration() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      throw std::runtime_error("Failed to initialize GLAD");
    }

    glViewport(0, 0, vidmode->width, vidmode->height);
    glEnable(GL_DEPTH_TEST);
  };

  // Window configuration
  void windowInicialization() {
    monitor = glfwGetPrimaryMonitor();
    vidmode = glfwGetVideoMode(monitor);

    GLFWwindow *w = glfwCreateWindow(vidmode->width, vidmode->height,
                                     "Default title", nullptr, nullptr);
    glfwSetWindowUserPointer(w, this);
    window = w;

    if (window == nullptr) {
      glfwTerminate();
      throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetKeyCallback(window, keyboardCallback);
  };

  // GLFW start configuration
  void glfwConfiguration() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
  };

  void calculateDeltaTime() {
    auto currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
  };

  static void mouseCallback(GLFWwindow *window, double xPos, double yPos) {
    auto events_buffer = &core::getEvents();
    events_buffer->push_back({EventType::Mouse, xPos, yPos});
  }

  static void keyboardCallback(GLFWwindow *window, int key, int scancode,
                               int action, int mods) {
    auto events_buffer = &core::getEvents();
    events_buffer->push_back(
        {EventType::Keyboard, (double)key, (double)action});
  };

  static void framebufferSizeCallback(GLFWwindow *window, int width,
                                      int height) {
    glViewport(0, 0, width, height);
  };
};

WindowSystem::WindowSystem() : impl(std::make_unique<WindowSystemImpl>()) {};

WindowSystem::~WindowSystem() = default;

void WindowSystem::swapBuffers() {
  glfwSwapBuffers(impl->window);
  glfwPollEvents();
  impl->calculateDeltaTime();
}

void WindowSystem::closeWindow() { glfwTerminate(); }

void WindowSystem::setTitle(const char *title) {
  glfwSetWindowTitle(impl->window, title);
}

float WindowSystem::getDeltaTime() const { return impl->deltaTime; }

float WindowSystem::getTotalTime() const {
  return static_cast<float>(glfwGetTime());
}

} // namespace oriongl::core
