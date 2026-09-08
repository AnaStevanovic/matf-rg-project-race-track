//#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoords = aTexCoords;

    gl_Position = projection * view * vec4(FragPos, 1.0);
}


//#shader fragment
#version 330 core

struct DirectionalLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform DirectionalLight dirLight;
uniform vec3 viewPos;

uniform sampler2D texture_diffuse1;

void main() {
    vec3 baseColor = texture(texture_diffuse1, TexCoords).rgb;
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-dirLight.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 ambient = dirLight.ambient * baseColor;
    vec3 diffuse = dirLight.diffuse * diff * baseColor;
    vec3 viewDir =
        normalize(viewPos - FragPos);
    vec3 reflectDir =
        reflect(-lightDir, norm);

    float spec =
        pow(
            max(dot(viewDir, reflectDir), 0.0),
            32.0
        );

    vec3 specular =
        dirLight.specular * spec;

    vec3 result =
        ambient + diffuse + specular;

    FragColor = vec4(result, 1.0);
}