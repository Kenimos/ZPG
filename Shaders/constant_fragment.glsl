#version 330 core

out vec4 outColor;

uniform vec3 materialColor;

void main()
{
    outColor = vec4(materialColor, 1.0);
}
