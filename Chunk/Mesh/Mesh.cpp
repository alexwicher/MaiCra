#include <glad/glad.h>
#include "Mesh.h"

Mesh::Mesh(const std::vector<Vertex> &vertices, const std::vector<Texture> &textures,
           const std::vector<unsigned int> &indices) : vertices(vertices), textures(textures), indices(indices) {

    initMesh();
}

void Mesh::initMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) 0);

    // texture attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex,textCoords));

    glBindVertexArray(0);
}

void Mesh::draw(ShaderLoader &shader) {
//    for(unsigned int i = 0; i < textures.size(); i++)
//    {
//        glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
//        // retrieve texture number (the N in diffuse_textureN)
//        // now set the sampler to the correct texture unit
//        glUniform1i(glGetUniformLocation(shader.getShaderProgram(), ("texture" + number).c_str()), i);
//        // and finally bind the texture
//        glBindTexture(GL_TEXTURE_2D, textures[i].id);
//    }

    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // always good practice to set everything back to defaults once configured.
    glActiveTexture(GL_TEXTURE0);

}
