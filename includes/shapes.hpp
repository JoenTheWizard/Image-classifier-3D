#ifndef SHAPES_H
#define SHAPES_H

#include <glm/glm.hpp>
#include "shader.hpp"

//Abstract shape, inherited only for primitive objects (Cube, pyramid, etc)
class Shape {
public:
    virtual void draw(Shader& shader) = 0;
    virtual const glm::vec3& getPosition() const { return position; }
    virtual void rotate(float angle, glm::vec3 axis) { 
        model = glm::rotate(model, glm::radians(angle), axis);
    }
    virtual void clean() = 0;
protected:
    //Store vertex buffer and array objects
    unsigned int VAO, VBO;
    //Store position vector and model matrix of the object in worldspace
    glm::vec3 position;
    glm::mat4 model;
};

#endif