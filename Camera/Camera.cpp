#include <glm/gtc/matrix_transform.hpp>
#include <cstdio>
#include <SDL2/SDL_mouse.h>
#include "Camera.h"
#include "../gameSettings.h"

glm::mat4 Camera::getView() {
    return glm::lookAt(pos, pos + front, up);
}

glm::mat4 Camera::getProjection() {
    return glm::perspective(glm::radians(FOV), (float) SCR_WIDTH / (float) SCR_HEIGHT, 0.1f, LINE_OF_SIGHT);
}

Camera::Camera(const glm::vec3 &pos) : pos(pos) {
    updateCameraVectors();
    up = glm::vec3(0, 1, 0);
    yaw = -90;
    pitch = 0;
    front = glm::vec3(0, 0, -1);
    worldUp = up;

}

void Camera::moveBackward(float deltaTime) {
    pos -= (deltaTime * CAM_SPEED) * front;
}

void Camera::moveLeft(float deltaTime) {
    pos -= (deltaTime * CAM_SPEED) * right;
}

void Camera::moveFoward(float deltaTime) {
    pos += (deltaTime * CAM_SPEED) * front;
}

void Camera::moveRight(float deltaTime) {
    pos += (deltaTime * CAM_SPEED) * right;
}


void Camera::motion(float x, float y, SDL_Window *window) {

    int midX = SCR_WIDTH / 2, midY = SCR_HEIGHT / 2;
    SDL_WarpMouseInWindow(window, midX, midY);
    yaw += MOUSE_SENSITIVITY * (x - midX);
    pitch -= MOUSE_SENSITIVITY * (y - midY);

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;
    updateCameraVectors();
}

void Camera::updateCameraVectors() {
    // calculate the new Front vector
    glm::vec3 frontAux;
    frontAux.x = glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
    frontAux.y = glm::sin(glm::radians(pitch));
    frontAux.z = glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
    front = glm::normalize(frontAux);
    right = glm::normalize(glm::cross(front,
                                      worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    up = glm::normalize(glm::cross(right, front));
}

const glm::vec3 &Camera::getPos() const {
    return pos;
}

void Camera::setPos(const glm::vec3 &pos) {
    Camera::pos = pos;
}


