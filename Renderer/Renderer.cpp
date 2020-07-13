#include "Renderer.h"
#include "../Shader/ShaderLoader.h"
#include <glad/glad.h>


std::array<unsigned int, 6> Renderer::initVertexBuffs(std::array<std::array<float,20>, 6> cubeMap) {
    std::array<unsigned int, 6> buffers{};
    for (int i = 0; i < cubeMap.size(); ++i) {
        unsigned int VBO, VAO;
        std::array<float,20> square = cubeMap[i];
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, square.size()* sizeof(float), &square[0], GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);

        // texture coord attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
        glEnableVertexAttribArray(1);


        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        buffers[i] = VAO;
    }

    return buffers;
}

void Renderer::renderCube(std::array<unsigned int , 6> VAOList,std::array<unsigned int , 6> texIDsList, ShaderLoader shader) {
    for (int i = 0; i < 6; ++i) {
        unsigned int texID = texIDsList[i] ,VAO = VAOList[i];

        glBindTexture(GL_TEXTURE_2D, texID);
        shader.use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_POLYGON, 0, 4);
    }
}
