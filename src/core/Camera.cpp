#include "Camera.h"
#include <InputSystemForward.h>
#include <glm/gtc/matrix_transform.hpp>

namespace {
constexpr const float aspect_ratio_array[] = {1920.0f / 1080.0f,
                                              1280.0f / 920.0f};
}

namespace oriongl::core {
Camera::Camera(float fovy, ratio::AspectRatio ratio, float near, float far)
    : perspective(
          glm::perspective(fovy, aspect_ratio_array[ratio], near, far)) {};

void Camera::setFront(float value) { pos += front * cameraSpeed * value; }

void Camera::setBack(float value) { pos -= front * cameraSpeed * value; }

void Camera::setLeft(float value) {
  pos -= glm::normalize(glm::cross(front, up)) * cameraSpeed * value;
}

void Camera::setRight(float value) {
  pos += glm::normalize(glm::cross(front, up)) * cameraSpeed * value;
}

void Camera::setUp(float value) { pos += up * cameraSpeed * value; }

void Camera::setDown(float value) { pos -= up * cameraSpeed * value; }

glm::mat4 &Camera::getView() { return view; }

glm::mat4 &Camera::getPerspective() { return perspective; }

glm::vec3 &Camera::getViewPosition() { return pos; }

void Camera::updateView() { view = glm::lookAt(pos, pos + front, up); }

void Camera::updatePerspective() {
  perspective = glm::perspective(45.0f, 1280.0f / 960.0f, 0.1f, 300.0f);
}

void Camera::lookAt(float x, float y) {
  if (firstMouse) {
    lastX = x;
    lastY = y;
    firstMouse = false;
    return;
  }

  float deltaYaw = (x - lastX) * sensitivity;
  float deltaPitch = (lastY - y) * sensitivity;

  if (deltaPitch == 0 && deltaYaw == 0)
    return;

  yaw += deltaYaw;
  pitch += deltaPitch;

  if (pitch > 89.0f)
    pitch = 89.0f;
  if (pitch < -89.0f)
    pitch = -89.0f;

  glm::vec3 direction;
  direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  direction.y = sin(glm::radians(pitch));
  direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

  front = glm::normalize(direction);
  lastX = x;
  lastY = y;
}

void Camera::processCommands(float deltaTime) {
  const auto &commands = getCommands();

  for (const auto &command : commands) {
    switch (command.action) {
    case Action::MoveForward:
      setFront(deltaTime);
      break;
    case Action::MoveBackward:
      setBack(deltaTime);
      break;
    case Action::MoveLeftward:
      setLeft(deltaTime);
      break;
    case Action::MoveRightward:
      setRight(deltaTime);
      break;
    case Action::MoveUpward:
      setUp(deltaTime);
      break;
    case Action::MoveDownward:
      setDown(deltaTime);
      break;
    case Action::LookAt:
      lookAt(command.value[0], command.value[1]);
      break;
    default:;
    }
  }
  updateView();
}
} // namespace oriongl::core
