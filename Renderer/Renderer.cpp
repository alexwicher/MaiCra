#include "Renderer.h"
#include "../Shader/ShaderLoader.h"
#include "../gameSettings.h"
#include "../Camera/Camera.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void
Renderer::renderCube(std::array<unsigned int, 6> VAOList, std::array<unsigned int, 6> textures, ShaderLoader shader,
                     Camera camera) {
    shader.use();
    glm::mat4 projection = camera.getProjection();
    shader.setMat4("projection", projection);

    // modelView transformation
    glm::mat4 view = camera.getView();
    shader.setMat4("modelView", view);

    for (int i = 0; i < 6; ++i) {
        unsigned int texID = textures[i], VAO = VAOList[i];
        if (VAO != 0) {
            glBindTexture(GL_TEXTURE_2D, texID);
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
        }
    }
}
