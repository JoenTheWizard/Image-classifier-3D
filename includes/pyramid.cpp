#include "pyramid.hpp"

Pyramid::Pyramid(float x, float y, float z) {
    position = glm::vec3(x,y,z);

    float vertices[] = {
        //Positions (X,Y,Z)   //Normal vectors
         -0.5f, -0.5f, 0.0f,  0.0f, 0.447214f, 0.894427f,
          0.0f, 0.5f, -0.5f,  0.0f, 0.447214f, 0.894427f,
          0.5f, -0.5f, 0.0f,  0.0f, 0.447214f, 0.894427f,
         
        //Left Side Triangle
         -0.5f, -0.5f, 0.0f,  -0.894427f, 0.447214f, 0.f,
         -0.5f, -0.5f, -1.0f, -0.894427f, 0.447214f, 0.f,
          0.0f, 0.5f, -0.5f,  -0.894427f, 0.447214f, 0.f,

        //Back side triangle
        -0.5f, -0.5f, -1.0f, 0.f, 0.447214, -0.894427,
         0.0f,  0.5f, -0.5f, 0.f, 0.447214, -0.894427,
         0.5f, -0.5f, -1.0f, 0.f, 0.447214, -0.894427,

        //Right side triangle
         0.5f, -0.5f, -1.0f,  0.894427f, 0.447214f, 0.f,
         0.0f,  0.5f, -0.5f,  0.894427f, 0.447214f, 0.f,
         0.5f, -0.5f,  0.0f,  0.894427f, 0.447214f, 0.f,

        //Bottom triangle 1
        -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.f,
        -0.5f, -0.5f, -1.0f,  0.0f, -1.0f, 0.f,
         0.5f, -0.5f, -1.0f,  0.0f, -1.0f, 0.f,

        //Bottom triangle 2
        -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.f,
         0.5f, -0.5f, -1.0f,  0.0f, -1.0f, 0.f,
         0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //Pass and enable position (layout (location = 0)) 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //Pass and enable normal vectors used for lighting (layout (location = 1))
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //Rebind default vertex buffer and array objects (aka unbind)
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Pyramid::draw(Shader& shader) {
    glBindVertexArray(VAO);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    shader.setMat4("model", model);

    glDrawArrays(GL_TRIANGLES, 0, 18);
    
    glBindVertexArray(0);
}

void Pyramid::clean() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}