#version 330 core

layout(location=0) in vec3 vertexPosition;
layout(location=1) in vec3 vertexColor;
layout(location=2) in vec3 vertexNormal;
layout(location = 3) in vec2 inUV;

uniform mat4 transformation;
uniform mat4 modelMatrix;
out vec3 fragPos;
out vec3 fragNormal;
out vec2 UV;

void main() {
    vec4 world = modelMatrix * vec4(vertexPosition,1.0);
    fragPos     = world.xyz;
    fragNormal  = mat3(transpose(inverse(modelMatrix))) * vertexNormal;
    UV = inUV;
    gl_Position = transformation * vec4(vertexPosition,1.0);
}
