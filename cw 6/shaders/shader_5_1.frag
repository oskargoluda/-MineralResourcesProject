#version 330 core

uniform vec3 cameraPos;
uniform float shininess;
uniform sampler2D diffuseTexture;
uniform vec3 ambientLight;

uniform int numLights;
uniform vec3 lightDirs[6];

in vec3 fragPos;
in vec3 fragNormal;
in vec2 UV;

out vec4 outColor;

void main() {
    vec3 albedo = texture(diffuseTexture, UV).rgb;
    vec3 texColor = texture(diffuseTexture, UV).rgb;
    vec3 N = normalize(fragNormal);
    vec3 V = normalize(cameraPos - fragPos);
    vec3 result = ambientLight;

    for (int i = 0; i < numLights; ++i) {
        vec3 L = normalize(-lightDirs[i]);
        vec3 R = reflect(-L, N);

        float diff = max(dot(N, L), 0.0);
        float spec = pow(max(dot(R, V), 0.0), shininess);

        result += albedo * diff + vec3(1.0) * spec;
    }

    outColor = vec4(result, 1.0);
}
