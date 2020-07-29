#include "Renderer.h"
#include "../Shader/ShaderLoader.h"
#include "../gameSettings.h"
#include "../Camera/Camera.h"
#include "../Cube/Cube.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


void Renderer::renderCubes(unsigned int textureArray, ShaderLoader *shader, Camera *camera) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader->use();
    glm::mat4 projection = camera->getProjection();
    shader->setMat4("projection", projection);

    // modelView transformation
    glm::mat4 view = camera->getView();
    shader->setMat4("modelView", view);

    for (int i = 0; i < facesVAOs.size(); ++i) {
        unsigned int VAO = facesVAOs[i];
        glBindVertexArray(VAO);
        glBindTexture(GL_TEXTURE_2D_ARRAY, textureArray);
        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0, static_cast<GLsizei>(faceOffsets[i].size()));
    }
}

void Renderer::initCubeInstancing(ShaderLoader *shader) {

    shader->use();
    shader->setInt("texture1", 0);
    float s = CUBE_SIZE;
    std::array<float, 20> front = {
            -s, s, -s, 0.0f, 1.0f,  //F-A -s s 0
            s, s, -s, 1.0f, 1.0f,  //F-B s s 1
            s, -s, -s, 1.0f, 0.0f,  //F-C s -s 2
            -s, -s, -s, 0.0f, 0.0f  //F-D -s -s 3
    };
    std::array<float, 20> back = {
            -s, s, s, 0.0f, 1.0f, //B-A -s s 4
            s, s, s, 1.0f, 1.0f,  //B-B s s 5
            s, -s, s, 1.0f, 0.0f, //B-C s- s 6
            -s, -s, s, 0.0f, 0.0f  //B-D -s -s 7
    };
    std::array<float, 20> right = {
            s, s, -s, 0.0f, 1.0f, //F-B
            s, s, s, 1.0f, 1.0f, //B-B
            s, -s, s, 1.0f, 0.0f, //B-C
            s, -s, -s, 0.0f, 0.0f  //F-C
    };
    std::array<float, 20> left = {
            -s, s, s, 0.0f, 1.0f, //B-A
            -s, s, -s, 1.0f, 1.0f, //F-A
            -s, -s, -s, 1.0f, 0.0f, //F-D
            -s, -s, s, 0.0f, 0.0f  //B-D
    };
    std::array<float, 20> top = {
            -s, s, s, 0.0f, 1.0f, //B-A
            s, s, s, 1.0f, 1.0f, //B-B
            s, s, -s, 1.0f, 0.0f, //F-B
            -s, s, -s, 0.0f, 0.0f  //F-A
    };
    std::array<float, 20> bottom = {
            -s, -s, -s, 0.0f, 1.0f,//F-D
            s, -s, -s, 1.0f, 1.0f,//F-C
            s, -s, s, 1.0f, 0.0f,//B-C
            -s, -s, s, 0.0f, 0.0f //B-D
    };
    unsigned short indices[] = {
            0, 1, 3,
            1, 2, 3
    };
    std::array<std::vector<glm::vec4>, 6> offsets;
    std::array<std::array<float, 20>, 6> cubeMap = {front, back, right, left, top, bottom};
    for (std::pair<std::string, Cube *> hash : cubeList) {
        killUselessNeighbours(hash.second);
    }
    // Setting offsets up
    for (auto &i : cubeList) {
        Cube cube = *i.second;
        float offx = cube.cubPos.x, offy = cube.cubPos.y, offz = cube.cubPos.z;
        for (int j = 0; j < cube.renderFace.size(); ++j)
            if (cube.renderFace[j])
                faceOffsets[j].emplace_back(glm::vec4(offx, offy, offz, cube.getTextureArrayIndexs()[j]));

    }

    for (int k = 0; k < cubeMap.size(); ++k) {
        unsigned int VBO, VAO, EBO, IBO;
        std::array<float, 20> face = cubeMap[k];
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
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);

        //texture coord attribute
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));

        // also set instance data
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, IBO); // this attribute comes from a different vertex buffer
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glVertexAttribDivisor(2, 1); // tell OpenGL this is an instanced vertex attribute.
        facesVAOs[k] = VAO;
        facesIBOs[k] = IBO;
    }
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
        cube->renderFace[i] = cubeList.find(checkSurroundings[i]) == cubeList.end();
}

Renderer::Renderer(const std::unordered_map<std::string, Cube *> &cubeList) : cubeList(cubeList) {}

void Renderer::reloadIBO(unsigned int IBOid, std::vector<glm::vec4> *e) {
    glBindBuffer(GL_ARRAY_BUFFER, IBOid);
    std::vector<glm::vec4> deepCopy = *e;
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * deepCopy.size(), &deepCopy[0], GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Renderer::deleteFace(glm::vec3 pos, int side) {
    std::vector<glm::vec4> *sideBuffer = &faceOffsets[side];
    for (int i = 0; i < sideBuffer->size(); i++) {
        glm::vec4 vec = faceOffsets[side][i];
        if (pos == glm::vec3(vec.x, vec.y, vec.z)) {
            sideBuffer->erase(sideBuffer->begin() + i);
            reloadIBO(facesIBOs[side], sideBuffer);
            break;
        }
    }
}

void Renderer::createFace(int x, int y, int z, int side) {
    Cube *cube = cubeList[std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z)];
    cube->renderFace[side] = true;
    std::vector<glm::vec4> *off = &faceOffsets[side];
    off->push_back(glm::vec4(x, y, z, cube->getTextureArrayIndexs()[side]));
    reloadIBO(facesIBOs[side], off);
}


void Renderer::addCube(Cube *cube) {
    int x = static_cast<int>(cube->cubPos.x), y = static_cast<int>(cube->cubPos.y), z = static_cast<int>(cube->cubPos.z), s = static_cast<int>(
            2 * CUBE_SIZE);
    std::string key = std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z);
    killUselessNeighbours(cube);
    cubeList[key] = cube;
    for (int i = 0; i < 6; ++i) {
        if (cube->renderFace[i]) {
            faceOffsets[i].push_back(glm::vec4(x, y, z, cube->getTextureArrayIndexs()[i]));
            reloadIBO(facesIBOs[i], &faceOffsets[i]);
        } else {
            glm::vec3 posAux = cube->cubPos;
            int i_n = i % 2 == 0 ? i + 1 : i - 1;
            if (i == 0 || i == 1)
                posAux.z = i == 0 ? z - s : z + s;
            if (i == 2 || i == 3)
                posAux.x = i == 2 ? x + s : x - s;
            if (i == 4 || i == 5)
                posAux.y = i == 4 ? y + s : y - s;
            deleteFace(posAux, i_n);
        }
    }
}

void Renderer::removeCube(Cube *cube) {
    int x = static_cast<int>(cube->cubPos.x), y = static_cast<int>(cube->cubPos.y), z = static_cast<int>(cube->cubPos.z), s = static_cast<int>(
            2 * CUBE_SIZE);
    std::string key = std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z);
    for (int i = 0; i < 6; i++) {
        std::vector<glm::vec4> e = faceOffsets[i];
        deleteFace(cube->cubPos, i);

        if (!cube->renderFace[i]) {
            int x_n = x, y_n = y, z_n = z, i_n = i % 2 == 0 ? i + 1 : i - 1;
            if (i == 0 || i == 1)
                z_n = i == 0 ? z - s : z + s;
            if (i == 2 || i == 3)
                x_n = i == 2 ? x + s : x - s;
            if (i == 4 || i == 5)
                y_n = i == 4 ? y + s : y - s;
            createFace(x_n, y_n, z_n, i_n);
        }

        cubeList.erase(key);
    }
}
