#ifndef SHADER_H_
#define SHADER_H_

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>

//Source(s): LearnOpenGL by Joey de Vries

class Shader {
public:
    //Shader program ID
    unsigned int ID;
    //Shader requries fragment and vertex shader input
    Shader(const char* vertexFilePath, const char* fragmentFilePath, const char* geometryFilePath = nullptr);
    //Use the shader
    void runShader();
    //These values are passed from the CPU to the GPU for the shader to execute
    //'name' is for the variable name passed and the 'value' copied to the GPU shader
    void setBool(const std::string &name, bool value) const; //Set bool value
    void setInt(const std::string &name, int value) const; //Set int value
    void setFloat(const std::string &name, float value) const; //Set float value
    void setVec3(const std::string &name, const glm::vec3 &value); //Set vec3 value (glm vec3) 
    void setVec3(const std::string &name, float val1, float val2, float val3); //Set vec3 value
    void setMat4(const std::string &name, const glm::mat4 &mat);
private:
    //Check for any compiling errors from the shader
    void checkCompileErrors(unsigned int shaderID, std::string type);
};

#endif
