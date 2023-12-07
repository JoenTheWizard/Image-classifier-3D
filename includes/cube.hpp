#ifndef CUBE_H
#define CUBE_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.hpp"

class Cube {
public:
    Cube(float x, float y, float z); //Initialize cube position (scalar values) 
    void draw(Shader& shader); //Draw cube with specified shader
    void clean();
private:
    unsigned int VAO, VBO;
    glm::vec3 position;
};

#endif