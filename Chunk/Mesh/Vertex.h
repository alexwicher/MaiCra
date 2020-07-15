#ifndef MAICRA_VERTEX_H
#define MAICRA_VERTEX_H


#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

class Vertex {
public:
    glm::vec3 pos;
    glm::vec2 textCoords;

    Vertex(const glm::vec3 &pos, const glm::vec2 &textCoords);

};


#endif //MAICRA_VERTEX_H
