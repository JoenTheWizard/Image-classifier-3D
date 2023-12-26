#ifndef PYRAMID_H
#define PYRAMID_H

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include "shapes.hpp"

class Pyramid : public Shape {
public:
    Pyramid(float x, float y, float z); //Initialize pyramid position (scalar values) 
    void draw(Shader& shader) override; //Draw pyramid with specified shader
    void clean() override;
};

#endif