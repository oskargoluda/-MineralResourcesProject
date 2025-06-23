#version 430 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;

uniform mat4 transformation;
uniform mat4 modelMatrix;

out vec3 vecNormal;
out vec3 worldPos;

void main()
{
    vec4 world = modelMatrix * vec4(vertexPosition, 1.0);
    worldPos = world.xyz;

    vecNormal =  vec3(modelMatrix * vec4(vertexNormal, 0.0));

    gl_Position = transformation * vec4(vertexPosition, 1.0);
}