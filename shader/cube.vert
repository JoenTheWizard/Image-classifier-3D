#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormals;

//Uniforms
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

//Output variables
out vec3 FragPos;
out vec3 Normals;

void main() {

    //Store the fragment positions
    FragPos = vec3(model * vec4(aPos, 1.0f));
    //Store the normals
    Normals = aNormals;

    gl_Position = projection * view * model * vec4(aPos, 1.0f);
}