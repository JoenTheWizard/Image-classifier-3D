#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "includes/shader.hpp"
#include "includes/camera.hpp"
#include "includes/cube.hpp"
#include "includes/pyramid.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

//Settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;

//Camera initialization
Camera camera(glm::vec3(0.f, 0.f, 3.f));
float lastX = SCR_WIDTH / 2.f;
float lastY = SCR_HEIGHT / 2.f;
bool firstMouse = true;

//Delta time and framing
float deltaTime = 0.0f;
float prevFrame = 0.0f;

int main(int argc, char* argv[]) {
    //GLFW: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //GLFW window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Interpolated Triangle", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); //GLFW Resize framebuffer
    glfwSetCursorPosCallback(window, mouse_callback); //GLFW take mouse movement

    //glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //Depth test enable
    glEnable(GL_DEPTH_TEST);

    //Build and compile our shader program
    Shader shader("shader/triangle.vert", "shader/triangle.frag");

    //Set up vertex data (and buffer(s)) and configure vertex attributes
    float vertices[] = {
        //Positions (X,Y,Z)  //Colors (R,G,B)
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  //Bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  //Bottom left
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   //Top 
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    //Bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, 
    //but this rarely happens. Modifying other VAOs requires a call to glBindVertexArray anyways so we
    //generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    Cube cube(0.f,1.f,0.f);
    Shader cube_shader("shader/cube.vert","shader/cube.frag");
    Shader cube_shader_normals("shader/cube_normals.vert","shader/cube_normals.frag", "shader/cube_normals.geom");

    //-- Render loop --
    while (!glfwWindowShouldClose(window)) {
        //Calculate the delta for time and for each frame
        float currentTime = static_cast<float>(glfwGetTime());
        deltaTime = currentTime - prevFrame;
        prevFrame = currentTime;

        //Process input
        processInput(window);

        //Begin the render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Render the triangle
        shader.runShader();
        //Set proj * view * model 
        //Pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        shader.setMat4("projection", projection);
        //Camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        shader.setMat4("view", view);
        //Model matrix
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0,0,0));
        shader.setMat4("model", model);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        cube_shader.runShader();
        cube_shader.setMat4("projection", projection);
        cube_shader.setMat4("view", view);
        cube_shader.setVec3("playerPos", camera.Position.x, camera.Position.y, camera.Position.z);
        cube.draw(cube_shader);

        //Normal vertices debug
        cube_shader_normals.runShader();
        cube_shader_normals.setMat4("projection", projection);
        cube_shader_normals.setMat4("view", view);
        cube_shader_normals.setMat4("model", glm::translate(glm::mat4(1.0), cube.getPosition()));
        cube.draw(cube_shader_normals);

        //glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //De-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    //GLFW: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

//Process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    //Movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);
}

//GLFW call: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    //Make sure the viewport matches the new window dimensions; note that width and 
    //height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

//GLFW: whenever the mouse moves, this callback is called
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; //Reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}