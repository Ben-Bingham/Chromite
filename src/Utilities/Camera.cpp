#include "camera.h"

#include <glm/ext/matrix_transform.hpp>

Camera::Camera(float movementSpeed, float lookSensitivity)
    : movementSpeed(movementSpeed), lookSensitivity(lookSensitivity) { }

glm::mat4 Camera::ViewMatrix() {
    return glm::lookAt(position, position + forward, up);
}
