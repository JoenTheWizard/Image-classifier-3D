#include "shader.hpp"

//Constructor: create the vertex and fragment shader programs
Shader::Shader(const char* vertexFilePath, const char* fragmentFilePath, const char* geometryFilePath) {
    //First the program needs to obtain the GLSL code from the vertex and fragment shader
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    std::ifstream gShaderFile;

    //Ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    gShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try  {
        //Open the shader files
        vShaderFile.open(vertexFilePath);
        fShaderFile.open(fragmentFilePath);
        std::stringstream vShaderStream, fShaderStream;

        //Read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        //Close file handlers
        vShaderFile.close();
        fShaderFile.close();

        //Convert stream into string
        vertexCode   = vShaderStream.str();
        fragmentCode = fShaderStream.str();

        //Load geometry shader if passed in
        if (geometryFilePath != nullptr) {
            gShaderFile.open(geometryFilePath);
            std::stringstream gShaderStream;
            gShaderStream << gShaderFile.rdbuf();
            gShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    }
    catch (std::ifstream::failure& e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    }

    //Obtain the code from the 
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    //Now to compile the shaders
    unsigned int vertex, fragment;

    //-- Vertex shader --
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    //-- Fragment Shader --
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    //if geometry shader is given, compile geometry shader
    unsigned int geometry;
    if(geometryFilePath != nullptr) {
        const char* gShaderCode = geometryCode.c_str();
        geometry = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometry, 1, &gShaderCode, NULL);
        glCompileShader(geometry);
        checkCompileErrors(geometry, "GEOMETRY");
    }

    //Combine them both and create the shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    if (geometryFilePath != nullptr)
        glAttachShader(ID, geometry);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");

    //Delete the shaders as they're linked into our program now and no longer necessary
    //annd free memory
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    if (geometryFilePath != nullptr)
        glDeleteShader(geometry);
}

//Use and run the shader program within the render loop
void Shader::runShader() {
    glUseProgram(ID);
}

//Set bool value
void Shader::setBool(const std::string &name, bool value) const {      
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
}

//Set int value
void Shader::setInt(const std::string &name, int value) const { 
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
}

//Set float value
void Shader::setFloat(const std::string &name, float value) const { 
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
}

//Set vec3 value
void Shader::setVec3(const std::string& name, float val1, float val2, float val3) {
    glUniform3f(glGetUniformLocation(ID, name.c_str()), val1, val2, val3);
}

//Set vec3 value (glm vec3)
void Shader::setVec3(const std::string &name, const glm::vec3 &value) { 
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
}

//Set mat4 value
void Shader::setMat4(const std::string &name, const glm::mat4 &mat) {
   glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

//Check the compile errors
void Shader::checkCompileErrors(unsigned int shader, std::string type) {
    int success;
    char infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}
