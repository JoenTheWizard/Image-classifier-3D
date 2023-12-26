#ifndef SHAPES_H
#define SHAPES_H

#include <glm/glm.hpp>
#include "shader.hpp"

//Abstract shape, inherited only for primitive objects (Cube, pyramid, etc)
class Shape {
public:
    virtual void draw(Shader& shader) = 0;
    virtual const glm::vec3& getPosition() const { return position; }
    virtual void clean() = 0;
protected:
    unsigned int VAO, VBO;
    glm::vec3 position;
};

#endif