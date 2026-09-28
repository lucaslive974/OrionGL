#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace oriongl::ratio {
enum AspectRatio : std::uint8_t { FullHD, HD };
}

namespace oriongl::core {

class Camera {
    glm::vec3 pos{0.0F, 0.0F, 30.0F};
    glm::vec3 front{0.0F, 0.0F, -1.0F};
    glm::vec3 up{0.0F, 1.0F, 0.0F};

    glm::mat4 view = glm::lookAt(pos, pos + front, up);
    glm::mat4 perspective = glm::perspective(45.0F, 1920.0F / 1080.0F, 0.1F, 1000.F);

    float cameraSpeed = 50.0F;

    float fov = 45.0F;
    float yaw = -90.0F;
    float pitch = 0.0F;

    float lastX;
    float lastY;

    float sensitivity = 0.1F;

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

    Camera(float fov, ratio::AspectRatio ratio, float near, float far);

    glm::mat4 &getView();

    glm::mat4 &getPerspective();

    glm::vec3 &getViewPosition();

    void processCommands(float deltaTime = 0.016F);
};
} // namespace oriongl::core
