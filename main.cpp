#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <getopt.h>

#include "includes/shader.hpp"
#include "includes/camera.hpp"
#include "includes/cube.hpp"
#include "includes/pyramid.hpp"
#include "includes/image_classifier.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
int parse_arguments(int argc, char* argv[], ImageClassifier* img_classifier);

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

//Argument parsing
extern char *optarg;
extern int optind, opterr, optopt;

int main(int argc, char* argv[]) {
    //Pass in the arguments
    ImageClassifier img;
    if (parse_arguments(argc, argv, &img)) {
        return -1;
    }

    //Obtain the maximum value obtained from propagated network. 
    //Remember the output neurons should always have 1 row
    size_t output_neurons_max_index = img.classify_max_column_index();

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

    //Initialize object list for classification. The index will be chosen from the neural network
    std::vector<Shape*> shapes;
    shapes.push_back(new Cube(0.f,0.f,0.f));
    shapes.push_back(new Pyramid(0.f,0.f,0.5f));

    //Build and compile our shader program
    Shader cube_shader("shader/cube.vert","shader/cube.frag");
    Shader cube_shader_normals("shader/cube_normals.vert","shader/cube_normals.frag", "shader/cube_normals.geom");

    //You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, 
    //but this rarely happens. Modifying other VAOs requires a call to glBindVertexArray anyways so we
    //generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

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

        //Set the projection and view matricies
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();

        //Render the object depending on the classification from the neural network
        cube_shader.runShader();
        cube_shader.setMat4("projection", projection);
        cube_shader.setMat4("view", view);
        cube_shader.setVec3("playerPos", camera.Position.x, camera.Position.y, camera.Position.z);
        float angle = deltaTime * 45.f;
        shapes[output_neurons_max_index]->rotate(angle, glm::vec3(0,1,0));
        shapes[output_neurons_max_index]->draw(cube_shader);

        //Normal vertices debug
        cube_shader_normals.runShader();
        cube_shader_normals.setMat4("projection", projection);
        cube_shader_normals.setMat4("view", view);
        shapes[output_neurons_max_index]->draw(cube_shader_normals);

        //GLFW: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //De-allocate all resources once they've outlived their purpose
    for (Shape* shape : shapes) {
        delete shape;
    }

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

//This function will need further improvements (current implementation is temporary for now)
//Could use input file handling
int parse_arguments(int argc, char* argv[], ImageClassifier* img_classifier) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " [options...] <input-file>\n";
        return -1;
    }

    int opt;
    char *weightsFile = nullptr, *outputFile = nullptr;
    bool canPropgate = false;

    //If one were to pass '-l weights.data -t dataset/shapes.data', train_from_dataset_load_weights() and
    //load_weights() together read and load the weights file twice. Not bothered right now to fix.
    while ((opt = getopt(argc, argv, "l:t:o:")) != -1) {
        switch (opt) {
            case 'l':
                img_classifier->load_weights(optarg);
                weightsFile = optarg; canPropgate = true;
                break;
            case 't':
                if (weightsFile) {
                    printf("[+] Training network on loaded weights file '%s'\n", weightsFile);
                    img_classifier->train_from_dataset_load_weights(optarg, weightsFile);
                } else {
                    printf("[+] Training network on randomized weights\n");
                    img_classifier->train_from_dataset(optarg);
                }
                canPropgate = true;
                break;
            case 'o':
                //img_classifier->save_weights(optarg);
                outputFile = optarg;
                break;
            case '?':
                std::cerr << "[-] Invalid option: " << (char)optopt << "\n";
                return -1;
            default:
                abort();
        }
    }

    //Save file (via '-o' argument)
    if (outputFile)
        img_classifier->save_weights(outputFile);

    //If input doesn't contain input file (input neurons from 28x28 file)
    if (optind >= argc) {
        std::cerr << "[!] No input file provided for forward pass... skipping..." << std::endl;
        return -1;
    }

    //If no weights and neurons have been initialized
    if (!canPropgate) {
        std::cerr << "[-] Please provide arguments for the model to train on..." << std::endl;
        return -1;
    }
    img_classifier->forward_propagate_img(argv[optind]);

    return 0;
}