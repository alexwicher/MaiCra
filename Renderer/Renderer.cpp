#include "Renderer.h"
#include "../Shader/ShaderLoader.h"
#include "../gameSettings.h"
#include "../Camera/Camera.h"
#include "../Control/Control.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


void Renderer::renderCubes(unsigned int textureArray, ShaderLoader *shader,
                           Camera *camera, bool *loop, SDL_Window *window, unsigned int *deltaTime, Control *control) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    control->handleCamera(camera, loop, window, *deltaTime);
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
        glDrawArraysInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, static_cast<GLsizei>(faceOffsets[i].size()));
    }
}

void Renderer::initCubeInstancing(std::vector<Cube * > cubeList, ShaderLoader *shader) {

    shader->use();
    shader->setInt("texture1", 0);
    float s = 1;
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

    // Setting offsets up
    for (unsigned int i = 0; i < cubeList.size(); i++) {
        Cube cube = *cubeList[i];
        float offx = cube.cubPos.x, offy = cube.cubPos.y, offz = cube.cubPos.z;
        for (int j = 0; j < cube.getRenderFace().size(); ++j)
            if (cube.getRenderFace()[j])
                faceOffsets[j].emplace_back(glm::vec4(offx, offy, offz, cube.getTextureArrayIndexs()[j]));
    }

    for (int k = 0; k < cubeMap.size(); ++k) {
        unsigned int VBO, VAO, EBO, IBO;
        std::array<float, 20> face = cubeMap[k];
        // store instance data in an array buffer
        // --------------------------------------
        glGenBuffers(1, &IBO);
        glBindBuffer(GL_ARRAY_BUFFER, IBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * faceOffsets[k].size(), &faceOffsets[k][0], GL_STATIC_DRAW);
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
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) 0);

        //texture coord attribute
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));

        // also set instance data
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, IBO); // this attribute comes from a different vertex buffer
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glVertexAttribDivisor(2, 1); // tell OpenGL this is an instanced vertex attribute.
        facesVAOs[k] = VAO;
    }
}
