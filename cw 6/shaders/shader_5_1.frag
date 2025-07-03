#version 330 core

uniform vec3 cameraPos;
uniform float shininess;
uniform sampler2D diffuseTexture;
uniform vec3 ambientLight;
uniform int numLights;
uniform vec3 lightDirs[6];
uniform vec3 baseColor;
uniform int useTexture;

uniform int clippingEnabled;
uniform vec4 clippingPlane;  // Ax + By + Cz + D = 0

in vec3 fragPos;
in vec3 fragNormal;
in vec2 UV;

out vec4 outColor;

void main() {

    if (clippingEnabled == 1) {
        float distance = dot(clippingPlane.xyz, fragPos) + clippingPlane.w;
        if (distance < 0.0) {
            discard; // jesli fragment jest po zlej stronie plaszczyzny to discard
        }
    }

    vec3 albedo;
    
    if (useTexture == 1) {
        albedo = texture(diffuseTexture, UV).rgb;
    } else {
        albedo = baseColor;
    }
    
    vec3 N = normalize(fragNormal);
    vec3 V = normalize(cameraPos - fragPos);
    vec3 result = ambientLight * albedo;

    for (int i = 0; i < numLights; ++i) {
        vec3 L = normalize(-lightDirs[i]);
        vec3 R = reflect(-L, N);
        float diff = max(dot(N, L), 0.0);
        float spec = pow(max(dot(R, V), 0.0), shininess);
        result += albedo * diff + vec3(0.3) * spec;
    }

    outColor = vec4(result, 1.0);
}
