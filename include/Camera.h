#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace oriongl::ratio {
enum AspectRatio : unsigned int { FullHD, HD };
}

namespace oriongl::core {

class Camera {
  glm::vec3 pos{0.0f, 0.0f, 30.0f};
  glm::vec3 front{0.0f, 0.0f, -1.0f};
  glm::vec3 up{0.0f, 1.0f, 0.0f};

  glm::mat4 view = glm::lookAt(pos, pos + front, up);
  glm::mat4 perspective =
      glm::perspective(45.0f, 1920.0f / 1080.0f, 0.1f, 1000.f);

  float cameraSpeed = 50.0f;

  float fov = 45.0f;
  float yaw = -90.0f;
  float pitch = 0.0f;

  float lastX;
  float lastY;

  float sensitivity = 0.1f;

  bool firstMouse = true;

  void updateView();

  void updatePerspective();

  void setFront(float value);

  void setBack(float value);

  void setLeft(float value);

  void setRight(float value);

  void setUp(float value);

  void setDown(float value);

  void lookAt(float x, float y);

public:
  Camera() = default;

  Camera(float fov, ratio::AspectRatio ratio, float near, float rear);

  glm::mat4 &getView();

  glm::mat4 &getPerspective();

  glm::vec3 &getViewPosition();

  void processCommands(float deltaTime = 0.016f);
};
} // namespace oriongl::core
