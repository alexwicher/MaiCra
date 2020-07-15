#ifndef MAICRA_MESH_H
#define MAICRA_MESH_H


#include "Vertex.h"
#include "Texture.h"
#include "../../Shader/ShaderLoader.h"
#include <vector>

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<Texture> textures;
    std::vector<unsigned int> indices;

    Mesh(const std::vector<Vertex> &vertices, const std::vector<Texture> &textures,
         const std::vector<unsigned int> &indices);

private:
    unsigned int VAO,VBO,EBO;
    void initMesh();
    void draw(ShaderLoader &shader);
};


#endif //MAICRA_MESH_H
