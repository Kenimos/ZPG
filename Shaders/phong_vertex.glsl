// phong_vertex.glsl
#version 330 core
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoords;

out vec3 fragPosition;
out vec3 fragNormal;
out vec2 texCoords;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
    vec4 worldPosition = modelMatrix * vec4(inPosition, 1.0);
    fragPosition = worldPosition.xyz;
    fragNormal = mat3(transpose(inverse(modelMatrix))) * inNormal;
    texCoords = inTexCoords;
    gl_Position = projectionMatrix * viewMatrix * worldPosition;
}
