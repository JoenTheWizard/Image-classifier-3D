#version 330 core
out vec4 FragColor;

//Take in input variables
in vec3 Normals;
in vec3 FragPos;

uniform vec3 playerPos;

void main() {

    //Object properties
    vec3 lightPos = vec3(0.f, 0.f, 2.f);
    vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
    vec3 objectColor = vec3(1.0, 0.5, 0.31);

    //Ambient lighting
    vec3 ambient = 0.4 * lightColor;

    //Diffusion lighting
    vec3 norm = normalize(Normals);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    //Specular lighting
    float specularStrength = 0.5;
    vec3 viewDir = normalize(playerPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    //Calculate the distance between the fragment and the light source
    float distance = length(lightPos - FragPos);

    //Calculate the attenuation factor
    //https://learnopengl.com/Lighting/Light-casters
    float k_atten = 1.0 + 0.09 * distance + 0.032; 
    float attenuation = 1.0 / (k_atten * (distance * distance));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    //Calculate total for Phong lighting
    vec3 result = (ambient + diffuse + specular) * objectColor;

    FragColor = vec4(result, 1);
}