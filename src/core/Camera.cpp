#include "Camera.h"
#include <InputSystemForward.h>
#include <algorithm>
#include <array>
#include <glm/gtc/matrix_transform.hpp>

namespace {
constexpr const std::array<float, 2> aspectRatioArray = {1920.0F / 1080.0F, 1280.0F / 920.0F};
}

namespace oriongl::core {
Camera::Camera(float fovy, ratio::AspectRatio ratio, float near, float far)
    : perspective(glm::perspective(fovy, aspectRatioArray[ratio], near, far)) {};

void Camera::setFront(float value) { pos += front * cameraSpeed * value; }

void Camera::setBack(float value) { pos -= front * cameraSpeed * value; }

void Camera::setLeft(float value) { pos -= glm::normalize(glm::cross(front, up)) * cameraSpeed * value; }

void Camera::setRight(float value) { pos += glm::normalize(glm::cross(front, up)) * cameraSpeed * value; }

void Camera::setUp(float value) { pos += up * cameraSpeed * value; }

void Camera::setDown(float value) { pos -= up * cameraSpeed * value; }

auto Camera::getView() -> glm::mat4 & { return view; }

auto Camera::getPerspective() -> glm::mat4 & { return perspective; }

auto Camera::getViewPosition() -> glm::vec3 & { return pos; }

void Camera::updateView() { view = glm::lookAt(pos, pos + front, up); }

void Camera::updatePerspective() { perspective = glm::perspective(45.0F, 1280.0F / 960.0F, 0.1F, 300.0F); }

void Camera::lookAt(float x, float y) { // NOLINT
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

    pitch = std::min(pitch, 89.0F);
    pitch = std::max(pitch, -89.0F);

    glm::vec3 direction;
    // NOLINTBEGIN
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    // NOLINTEND

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
            lookAt(command.value[0], command.value[1]); // NOLINT
            break;
        default:;
        }
    }
    updateView();
}
} // namespace oriongl::core
