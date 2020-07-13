#include "Renderer.h"
#include "../Shader/ShaderLoader.h"
#include <glad/glad.h>


unsigned int Renderer::initVertexBuffs(std::array<float, 96> cubeMap) {
    unsigned int VBO, VAO, EBO = 0;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, cubeMap.size(), cubeMap.data(), GL_STATIC_DRAW);

    // position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return VAO;
}

void Renderer::renderCube(unsigned int VAO, unsigned cubeTexID, ShaderLoader shader) {
    shader.use();
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTexID);
    glBindVertexArray(VAO);
    glDrawArrays(GL_QUADS, 0, 36);

}
