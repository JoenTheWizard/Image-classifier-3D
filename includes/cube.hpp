#ifndef CUBE_H
#define CUBE_H

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include "shapes.hpp"

class Cube : public Shape {
public:
    Cube(float x, float y, float z); //Initialize cube position (scalar values) 
    void draw(Shader& shader) override; //Draw cube with specified shader
    void clean() override;
};

#endif