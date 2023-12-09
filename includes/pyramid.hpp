#ifndef PYRAMID_H
#define PYRAMID_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.hpp"

class Pyramid {
public:
    Pyramid(float x, float y, float z); //Initialize pyramid position (scalar values) 
    void draw(Shader& shader); //Draw pyramid with specified shader
    void clean();
    const glm::vec3& getPosition() const { return position; }
private:
    unsigned int VAO, VBO;
    glm::vec3 position;
};

#endif