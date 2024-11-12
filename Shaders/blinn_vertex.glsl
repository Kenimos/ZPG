#version 330 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;

out vec3 fragPosition;
out vec3 fragNormal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
    vec4 worldPosition = modelMatrix * vec4(inPosition, 1.0);
    fragPosition = worldPosition.xyz;
    fragNormal = mat3(transpose(inverse(modelMatrix))) * inNormal;
    gl_Position = projectionMatrix * viewMatrix * worldPosition;
}
