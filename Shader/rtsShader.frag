#version 130

in vec3 coords;

out vec4 outputColor;

uniform samplerCube texture1;

void main()
{
	outputColor = texture(texture1, coords);
}
