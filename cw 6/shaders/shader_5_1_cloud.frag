#version 430 core

in vec3 worldPos;
in vec2 texCoord;

out vec4 outColor;
in vec3 vecNormal;

uniform vec3 lightPos;
uniform vec3 cameraPos;


void main() {
    outColor = vec4(1.0f);
}