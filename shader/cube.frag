#version 330 core
out vec4 FragColor;

//Take in input variables
in vec3 Normals;
in vec3 FragPos;

void main() {

    //Object properties
    vec3 lightPos = vec3(1.2f, 1.0f, 2.0f);
    vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
    vec3 objectColor = vec3(1.0, 0.5, 0.31);

    //Ambient lighting
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    //Diffusion lighting
    vec3 norm = normalize(Normals);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    //Calculate total
    vec3 result = (ambient + diffuse) * objectColor;

    FragColor = vec4(result,1);
}