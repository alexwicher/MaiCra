#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_system.h>
#include <SDL2/SDL.h>
#include <algorithm>
#include "Control.h"
#include "../Camera/Camera.h"
#include "../Renderer/Renderer.h"
#include "../gameSettings.h"

void Control::handleCamera(Camera *camera, bool *loop, SDL_Window *window, float deltaTime) {
    const Uint8 *keyboard = SDL_GetKeyboardState(nullptr);
    if (keyboard[SDL_SCANCODE_W]) {
        camera->moveFoward(deltaTime);
    }
    if (keyboard[SDL_SCANCODE_S]) {
        camera->moveBackward(deltaTime);
    }
    if (keyboard[SDL_SCANCODE_A]) {
        camera->moveLeft(deltaTime);
    }
    if (keyboard[SDL_SCANCODE_D]) {
        camera->moveRight(deltaTime);
    }
    if (keyboard[SDL_SCANCODE_F12]) {
        *loop = false;
        SDL_Quit();
    }
    if (keyboard[SDL_SCANCODE_DELETE]) {
        mouseIn = false;
        SDL_ShowCursor(SDL_ENABLE);
    }

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        int x, y;
        Uint32 mouse = SDL_GetMouseState(&x, &y);
        if (mouse == 1) {
            mouseIn = true;
            SDL_ShowCursor(SDL_DISABLE);
        }
        if (mouseIn) {
            camera->motion((float) x, (float) y, window);
        }
    }

}

void reloadIBO(unsigned int IBOid, std::vector<glm::vec4> *e) {
    glBindBuffer(GL_ARRAY_BUFFER, IBOid);
    std::vector<glm::vec4> deepCopy = *e;
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * deepCopy.size(), &deepCopy[0], GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void deleteFace(std::vector<glm::vec4> *e, Cube *cube, Renderer *renderer) {

    int x = static_cast<int>(cube->cubPos.x),
            y = static_cast<int>(cube->cubPos.y),
            z = static_cast<int>(cube->cubPos.z),
            s = static_cast<int>(CUBE_SIZE * 2);

    std::vector<glm::vec4> eValue = *e;

//    if (!cube->renderFace[0]) {
//        Cube *back = renderer->cubeList[std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z - s)];
//        back->renderFace[1] = true;
//        std::vector<glm::vec4> *off = &renderer->faceOffsets[1];
//        off->emplace_back(glm::vec4(x, y, z - s, back->getTextureArrayIndexs()[1]));
//        reloadIBO(renderer->facesIBOs[1], off);
//
//    }
//    if (!cube->renderFace[1]) {
//        Cube *front = renderer->cubeList[std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z + s)];
//        front->renderFace[0] = true;
//        std::vector<glm::vec4> *off = &renderer->faceOffsets[0];
//        off->emplace_back(glm::vec4(x, y, z + s, front->getTextureArrayIndexs()[0]));
//        reloadIBO(renderer->facesIBOs[0], off);
//    }
//    if (!cube->renderFace[2]) {
//        Cube *left = renderer->cubeList[std::to_string(x + s) + "," + std::to_string(y) + "," + std::to_string(z)];
//        left->renderFace[3] = true;
//        std::vector<glm::vec4> *off = &renderer->faceOffsets[3];
//        off->emplace_back(glm::vec4(x + s, y, z, left->getTextureArrayIndexs()[3]));
//        reloadIBO(renderer->facesIBOs[3], off);
//
//    }
//    if (!cube->renderFace[3]) {
//        Cube *right = renderer->cubeList[std::to_string(x - s) + "," + std::to_string(y) + "," + std::to_string(z)];
//        right->renderFace[2] = true;
//        std::vector<glm::vec4> *off = &renderer->faceOffsets[2];
//        off->emplace_back(glm::vec4(x - s, y, z, right->getTextureArrayIndexs()[2]));
//        reloadIBO(renderer->facesIBOs[2], off);
//
//    }
//    if (!cube->renderFace[4]) {
//        Cube *bottom = renderer->cubeList[std::to_string(x) + "," + std::to_string(y + s) + "," + std::to_string(z)];
//        bottom->renderFace[5] = true;
//        std::vector<glm::vec4> *off = &renderer->faceOffsets[5];
//        off->emplace_back(glm::vec4(x, y + s, z, bottom->getTextureArrayIndexs()[5]));
//        reloadIBO(renderer->facesIBOs[5], off);
//
//    }
//    if (!cube->renderFace[5]) {
//        Cube *top = renderer->cubeList[std::to_string(x) + "," + std::to_string(y - s) + "," + std::to_string(z)];
//        top->renderFace[4] = true;
//        std::vector<glm::vec4> *off = &renderer->faceOffsets[4];
//        off->emplace_back(glm::vec4(x, y - s, z, top->getTextureArrayIndexs()[4]));
//        reloadIBO(renderer->facesIBOs[4], off);
//
//    }

    for (int i = 0; i < e->size(); i++) {
        glm::vec4 vec = eValue[i];
        if (cube->cubPos == glm::vec3(vec.x, vec.y, vec.z)) {
            e->erase(e->begin() + i);
//            reloadIBO(renderer->facesIBOs[0], e);
            break;
        }
    }
}

void Control::handleRenderer(Renderer *renderer) {
    const Uint8 *keyboard = SDL_GetKeyboardState(nullptr);
    std::string key = std::to_string(0) + "," + std::to_string(0) + "," + std::to_string(0);
    if (keyboard[SDL_SCANCODE_E]) {
        if (renderer->cubeList.find(key) != renderer->cubeList.end()) {
            Cube *cube = renderer->cubeList[key];

            for (int i = 0; i < 6; i++) {
                std::vector<glm::vec4> *e = &renderer->faceOffsets[i];
                deleteFace(e, cube, renderer);
            }
            renderer->cubeList.erase(key);
        }
    }
}

Control::Control() {
    mouseIn = true;
    SDL_ShowCursor(SDL_DISABLE);

}
