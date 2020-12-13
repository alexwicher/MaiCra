#include "Renderer.h"
#include "../Shader/ShaderLoader.h"
#include "../gameSettings.h"
#include "../Camera/Camera.h"
#include "../Cube/Cube.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// renderQuad() renders a 1x1 XY quad in NDC
// -----------------------------------------
unsigned int quadVAO;
unsigned int quadVBO;

void renderQuad() {
    float quadVertices[] = {
            // positions        // texture Coords
            -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
            1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    };
    // setup plane VAO
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

void Renderer::renderCubes(unsigned int textureArray, ShaderLoader *shader, Camera *camera, glm::vec3 lightDirection,
                           ShaderLoader *depthShader, ShaderLoader *debugDepthQuad) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

    // 1. render depth of scene to texture (from light's perspective)
    // ---------------------------------------------------------
    glm::mat4 lightProjection, lightView;
    glm::mat4 lightSpaceMatrix;
    float near_plane = 1.0f, far_plane = 17.5f;
    lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    lightView = glm::lookAt(lightDirection, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
    lightSpaceMatrix = lightProjection * lightView;

    depthShader->use();
    depthShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);

    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glClear(GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, textureArray);
    for (int i = 0; i < facesVAOs.size(); ++i) {
        unsigned int VAO = facesVAOs[i];
        glBindVertexArray(VAO);
        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0, static_cast<GLsizei>(faceOffsets[i].size()));
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // reset viewport
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    // 2. render scene as normal using the generated depth/shadow map
    // --------------------------------------------------------------

    shader->use();
    shader->setVec3("light.direction", lightDirection);
    shader->setVec3("viewPos", camera->pos);
    shader->setMat4("lightSpaceMatrix", lightSpaceMatrix);


    // light properties
    shader->setVec3("light.ambient", glm::vec3(0.1, 0.1, 0.1));
    shader->setVec3("light.diffuse", glm::vec3(0.5, 0.5, 0.5));
    shader->setVec3("light.specular", glm::vec3(0, 0, 0));


    glm::mat4 projection = camera->getProjection();
    shader->setMat4("projection", projection);
    glm::mat4 view = camera->getView();
    shader->setMat4("view", view);
    glm::mat4 model = glm::mat4(1.0f);
    shader->setMat4("model", model);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, textureArray);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    for (int i = 0; i < facesVAOs.size(); ++i) {
        unsigned int VAO = facesVAOs[i];
        glBindVertexArray(VAO);
        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0, static_cast<GLsizei>(faceOffsets[i].size()));
    }


    // render Depth map to quad for visual debugging, uncomment to see depth frameBuffer
    // ---------------------------------------------
//    debugDepthQuad->use();
//    debugDepthQuad->setFloat("near_plane", near_plane);
//    debugDepthQuad->setFloat("far_plane", far_plane);
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, depthMap);
//    renderQuad();

}

void Renderer::initCubeInstancing(ShaderLoader *shader, ShaderLoader *debugDepthQuad) {

    shader->use();
    shader->setInt("texture1", 0);
    shader->setInt("shadowMap", 1);
    debugDepthQuad->use();
    debugDepthQuad->setInt("depthMap", 0);
    float s = CUBE_SIZE;
    const int size = 32;
    std::array<float, size> front = {
            -s, s, -s, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f,  //F-A -s s 0
            s, s, -s, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, //F-B s s 1
            s, -s, -s, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,  //F-C s -s 2
            -s, -s, -s, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f  //F-D -s -s 3
    };
    std::array<float, size> back = {
            -s, s, s, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, //B-A -s s 4
            s, s, s, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,  //B-B s s 5
            s, -s, s, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, //B-C s- s 6
            -s, -s, s, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f  //B-D -s -s 7
    };
    std::array<float, size> right = {
            s, s, -s, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, //F-B
            s, s, s, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, //B-B
            s, -s, s, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, //B-C
            s, -s, -s, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f  //F-C
    };
    std::array<float, size> left = {
            -s, s, s, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, //B-A
            -s, s, -s, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, //F-A
            -s, -s, -s, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, //F-D
            -s, -s, s, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f //B-D
    };
    std::array<float, size> top = {
            -s, s, s, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, //B-A
            s, s, s, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, //B-B
            s, s, -s, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, //F-B
            -s, s, -s, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,  //F-A
    };
    std::array<float, size> bottom = {
            -s, -s, -s, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,//F-D
            s, -s, -s, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f,//F-C
            s, -s, s, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f,//B-C
            -s, -s, s, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,//B-D
    };
    unsigned short indices[] = {
            0, 1, 3,
            1, 2, 3
    };
    std::array<std::vector<glm::vec4>, 6> offsets;
    std::array<std::array<float, size>, 6> cubeMap = {front, back, right, left, top, bottom};
    for (std::pair<std::string, Cube *> hash : cubeList) {
        killUselessNeighbours(hash.second);
    }
    // Setting offsets up
    for (auto &i : cubeList) {
        Cube cube = *i.second;
        float offx = cube.cubPos.x, offy = cube.cubPos.y, offz = cube.cubPos.z;
        for (int j = 0; j < 6; ++j)
            if (cube.shouldRenderFace(j))
                faceOffsets[j].emplace_back(glm::vec4(offx, offy, offz, cube.getTextureArrayIndexs()[j]));

    }

    for (int k = 0; k < cubeMap.size(); ++k) {
        unsigned int VBO, VAO, EBO, IBO;
        std::array<float, size> face = cubeMap[k];
        // store instance data in an array buffer
        // --------------------------------------
        glGenBuffers(1, &IBO);
        glBindBuffer(GL_ARRAY_BUFFER, IBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * faceOffsets[k].size(), &faceOffsets[k][0], GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glGenVertexArrays(1, &VAO);
        // set up vertex data (and buffer(s)) and configure vertex attributes
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, face.size() * sizeof(float), &face[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // position attribute
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);

        //texture coord attribute
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));

        //normal coord attibute
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (5 * sizeof(float)));
        // also set instance data
        glEnableVertexAttribArray(3);
        glBindBuffer(GL_ARRAY_BUFFER, IBO); // this attribute comes from a different vertex buffer
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glVertexAttribDivisor(3, 1); // tell OpenGL this is an instanced vertex attribute.
        facesVAOs[k] = VAO;
        facesIBOs[k] = IBO;
    }
    // configure depth map FBO
    // -----------------------
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    unsigned int depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);
    FBO = depthMapFBO;
    // create depth texture
    unsigned int depthMapAux;
    glGenTextures(1, &depthMapAux);
    depthMap = depthMapAux;
    glBindTexture(GL_TEXTURE_2D, depthMapAux);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT,
                 nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = {1.0, 1.0, 1.0, 1.0};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapAux, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::killUselessNeighbours(Cube *cube) {
    int x = static_cast<int>(cube->cubPos.x),
            y = static_cast<int>(cube->cubPos.y),
            z = static_cast<int>(cube->cubPos.z),
            s = static_cast<int>(CUBE_SIZE * 2);


    std::array<std::string, 6> checkSurroundings = {
            std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z - s), //front
            std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z + s), //back
            std::to_string(x + s) + "," + std::to_string(y) + "," + std::to_string(z), //right
            std::to_string(x - s) + "," + std::to_string(y) + "," + std::to_string(z), //left
            std::to_string(x) + "," + std::to_string(y + s) + "," + std::to_string(z), //top
            std::to_string(x) + "," + std::to_string(y - s) + "," + std::to_string(z) //bottom
    };

    for (int i = 0; i < 6; ++i)
        cube->setRenderFace(i, cubeList.find(checkSurroundings[i]) == cubeList.end());
}

Renderer::Renderer(const std::unordered_map<std::string, Cube *> &cubeList) : cubeList(cubeList) {}

void Renderer::reloadIBO(unsigned int IBOid, std::vector<glm::vec4> *e) {
    glBindBuffer(GL_ARRAY_BUFFER, IBOid);
    std::vector<glm::vec4> deepCopy = *e;
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * deepCopy.size(), &deepCopy[0], GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Renderer::deleteFace(int x, int y, int z, int side) {
    std::vector<glm::vec4> *sideBuffer = &faceOffsets[side];
    for (int i = 0; i < sideBuffer->size(); i++) {
        glm::vec4 vec = faceOffsets[side][i];
        if (glm::vec3(x, y, z) == glm::vec3(vec.x, vec.y, vec.z)) {
            sideBuffer->erase(sideBuffer->begin() + i);
            reloadIBO(facesIBOs[side], sideBuffer);
            break;
        }
    }
}

void Renderer::createFace(int x, int y, int z, int side) {
    Cube *cube = cubeList[std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z)];
    cube->setRenderFace(side, true);
    std::vector<glm::vec4> *off = &faceOffsets[side];
    off->push_back(glm::vec4(x, y, z, cube->getTextureArrayIndexs()[side]));
    reloadIBO(facesIBOs[side], off);
}


void Renderer::addCube(RaySelection *raySelection, unsigned short cubeType) {
    int s = (int) (2 * CUBE_SIZE);
    if (raySelection->cubeSelected) {
        glm::vec3 newCub = raySelection->cubeSelected->cubPos;
        int face = raySelection->faceSelected;
        if (face == 0 || face == 1) {
            newCub.z += face == 0 ? -s : s;
        }
        if (face == 2 || face == 3) {
            newCub.x += face == 2 ? -s : s;
        }
        if (face == 5 || face == 4) {
            newCub.y += face == 5 ? -s : s;
        }

        Cube *cube = new Cube(cubeType, newCub);
        if (cubeList.find(cube->getKey()) != cubeList.end())
            return;
        int x = static_cast<int>(cube->cubPos.x), y = static_cast<int>(cube->cubPos.y), z = static_cast<int>(cube->cubPos.z);
        killUselessNeighbours(cube);
        cubeList[cube->getKey()] = cube;
        for (int i = 0; i < 6; ++i) {
            if (cube->shouldRenderFace(i)) {
                faceOffsets[i].push_back(glm::vec4(x, y, z, cube->getTextureArrayIndexs()[i]));
                reloadIBO(facesIBOs[i], &faceOffsets[i]);
            } else {
                int x_n = x, y_n = y, z_n = z, i_n = i % 2 == 0 ? i + 1 : i - 1;
                if (i == 0 || i == 1)
                    z_n = i == 0 ? z - s : z + s;
                if (i == 2 || i == 3)
                    x_n = i == 2 ? x + s : x - s;
                if (i == 4 || i == 5)
                    y_n = i == 4 ? y + s : y - s;
                cubeList[std::to_string(x_n) + "," +
                         std::to_string(y_n) + "," +
                         std::to_string(z_n)]->setRenderFace(i_n, false);
                deleteFace(x_n, y_n, z_n, i_n);
            }
        }
    }
}

void Renderer::removeCube(std::string cubeKey) {
    if (cubeList.find(cubeKey) == cubeList.end())
        return;
    Cube *cube = cubeList[cubeKey];
    int x = static_cast<int>(cube->cubPos.x), y = static_cast<int>(cube->cubPos.y), z = static_cast<int>(cube->cubPos.z), s = static_cast<int>(
            2 * CUBE_SIZE);
    std::string key = std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z);
    for (int i = 0; i < 6; i++) {
        std::vector<glm::vec4> e = faceOffsets[i];
        deleteFace(x, y, z, i);

        if (!cube->shouldRenderFace(i)) {
            int x_n = x, y_n = y, z_n = z, i_n = i % 2 == 0 ? i + 1 : i - 1;
            if (i == 0 || i == 1)
                z_n = i == 0 ? z - s : z + s;
            if (i == 2 || i == 3)
                x_n = i == 2 ? x + s : x - s;
            if (i == 4 || i == 5)
                y_n = i == 4 ? y + s : y - s;
            createFace(x_n, y_n, z_n, i_n);
        }

        cubeList.erase(cube->getKey());
    }
}

RaySelection *Renderer::getCubeFromMouseRay(Camera *camera) {
    std::string key;
    auto *rayResult = new RaySelection();
    bool cubeFound = false;
    glm::vec3 partialRay = glm::vec3();
    for (int i = 1; i <= RAY_LENGH; ++i) {
        partialRay = camera->pos + (camera->front * ((float) i));
        int x = (int) (glm::round(partialRay.x)), y = (int) (glm::round(partialRay.y)), z = (int) (glm::round(
                partialRay.z));
        std::string aux = std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z);
        if (cubeList.find(aux) != cubeList.end()) {
            key = aux;
            cubeFound = true;
            break;
        }
    }
    if (cubeFound) {
        //TODO: retroRay precition must be improved ...
        glm::vec3 retroRay = (partialRay + (-camera->front) - cubeList[key]->cubPos);
        rayResult->cubeSelected = cubeList[key];
        rayResult->faceSelected = getFaceFromRetroRay(retroRay);
        return rayResult;
    } else
        return rayResult;
}

int Renderer::getFaceFromRetroRay(glm::vec3 retroRay) {
    int maxIdx = 0;
    float max = glm::abs(retroRay[0]);
    for (int i = 0; i < 2; ++i) {
        float next = glm::abs(retroRay[i + 1]);
        if (next > max) {
            max = next;
            maxIdx = i + 1;
        }
    }

    if (maxIdx == 0)
        return (retroRay[maxIdx] > 0) ? 3 : 2;
    if (maxIdx == 1)
        return (retroRay[maxIdx] > 0) ? 4 : 5;
    if (maxIdx == 2)
        return (retroRay[maxIdx] > 0) ? 1 : 0;

    return -1;
}
