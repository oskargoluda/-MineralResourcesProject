#version 430 core

float AMBIENT = 0.1;

uniform vec3 lightPos;
uniform vec3 cameraPos;
uniform float shininess = 15.0;

in vec3 vecNormal;
in vec3 worldPos;

out vec4 outColor;

void main()
{
    vec3 normal = normalize(vecNormal);

    // === Lighting vectors ===
    vec3 lightDir = normalize(lightPos - worldPos);
    vec3 viewDir  = normalize(cameraPos - worldPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    // === Lighting terms ===
    float diffuse = max(dot(normal, lightDir), 0.0);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess)*0.5f;

    // === Texture ===
    vec3 color = vec3(0.5f);
    vec3 surfaceColor = color;

    // === Combine lighting ===
    float lighting = clamp(AMBIENT + diffuse, 0.0, 1.0);

    outColor = vec4(color * lighting + spec * vec3(1.0), 1.0);
    //outColor = vec4((normal+1.0)*0.5, 1.0);
}
