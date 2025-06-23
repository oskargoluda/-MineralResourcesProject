#version 330 core

uniform vec3 cameraPos;
uniform float shininess;
uniform vec3 baseColor;

// Œwiat³o otoczenia
uniform vec3 ambientLight;

// Œwiat³a kierunkowe (maksymalnie 6)
uniform int numLights;
uniform vec3 lightDirs[6];

in vec3 fragPos;
in vec3 fragNormal;
in vec3 vColor;
in vec3 fragColor;
out vec4 FragColor;
out vec4 outColor;

void main() {
    vec3 albedo = baseColor;
    FragColor = vec4(fragColor, 1.0);

    vec3 N = normalize(fragNormal);
    vec3 V = normalize(cameraPos - fragPos);
    vec3 result = ambientLight; // zaczynamy od œwiat³a otaczaj¹cego

    for (int i = 0; i < numLights; ++i) {
        vec3 L = normalize(-lightDirs[i]); // œwiat³o kierunkowe (odwrotny kierunek)
        vec3 R = reflect(-L, N);

        float diff = max(dot(N, L), 0.0);
        float spec = pow(max(dot(R, V), 0.0), shininess);

        result += albedo * diff + vec3(1.0) * spec;
    }

    outColor = vec4(result, 1.0);
}
