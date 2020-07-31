#ifndef MAICRA_CAMERA_H
#define MAICRA_CAMERA_H


#include <glm/vec3.hpp>
#include <glm/detail/type_mat.hpp>
#include <glm/detail/type_mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <SDL2/SDL_video.h>

class Camera {
private:
    glm::vec3 right;
    glm::vec3 up;
    glm::vec3 worldUp;
    float yaw, pitch;

    glm::mat4 projection;

    void updateCameraVectors();

public:
    glm::vec3 front;
    glm::vec3 pos;

    explicit Camera(const glm::vec3 &pos);

    void motion(float x, float y, SDL_Window *window);

    void moveFoward(float deltaTime);

    void moveBackward(float deltaTime);

    void moveLeft(float deltaTime);

    void moveRight(float deltaTime);

    glm::vec3 getRayCoords();

    void setProjection(const glm::mat4 &projection);

    glm::mat4 getView();

    glm::mat4 getProjection();


};


#endif //MAICRA_CAMERA_H
