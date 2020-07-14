#ifndef MAICRA_CAMERA_H
#define MAICRA_CAMERA_H


#include <glm/vec3.hpp>
#include <glm/detail/type_mat.hpp>
#include <glm/detail/type_mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <SDL2/SDL_video.h>

class Camera {
private:
    glm::vec3 pos;
    glm::vec3 right;
    glm::vec3 up;
    glm::vec3 worldUp;
public:
    const glm::vec3 &getPos() const;

    void setPos(const glm::vec3 &pos);

private:
    glm::vec3 front;
    float yaw,pitch;

    void updateCameraVectors();

public:
    explicit Camera(const glm::vec3 &pos);

    void motion(float x,float y,SDL_Window * window);

    void moveFoward();
    void moveBackward();
    void moveLeft();
    void moveRight();

    glm::mat4 getView();
    glm::mat4 getProjection();


};


#endif //MAICRA_CAMERA_H
