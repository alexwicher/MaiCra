#version 450
layout (location = 0) in vec3 aPos;
layout (location = 3) in vec4 offset;

uniform mat4 lightSpaceMatrix;

void main()
{
    gl_Position = lightSpaceMatrix * vec4(aPos +offset.xyz, 1.0);
}