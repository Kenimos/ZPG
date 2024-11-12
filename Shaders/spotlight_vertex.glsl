#version 330 core

layout(location = 0) in vec3 aPos;    // Vertex position
layout(location = 1) in vec3 aNormal; // Vertex normal

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 FragPos;    // Position of the fragment in world space
out vec3 Normal;     // Normal of the fragment in world space

void main()
{
    FragPos = vec3(modelMatrix * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(modelMatrix))) * aNormal;  
    
    gl_Position = projectionMatrix * viewMatrix * vec4(FragPos, 1.0);
}
