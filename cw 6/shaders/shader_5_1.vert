#version 330 core

layout(location=0) in vec3 vertexPosition;
layout(location=1) in vec3 vertexColor;
layout(location=2) in vec3 vertexNormal;

uniform mat4 transformation;
uniform mat4 modelMatrix;
in vec3 color;
out vec3 fragColor;
out vec3 fragPos;
out vec3 fragNormal;
out vec3 vColor;

void main() {
    fragColor = color;
    vec4 world = modelMatrix * vec4(vertexPosition,1.0);
    fragPos     = world.xyz;
    fragNormal  = mat3(transpose(inverse(modelMatrix))) * vertexNormal;
    vColor      = vertexColor;
    gl_Position = transformation * vec4(vertexPosition,1.0);
}
