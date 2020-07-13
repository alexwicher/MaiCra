#version 130

in vec2 coords;

out vec4 outputColor;

uniform sampler2D texture1;

void main()
{
	outputColor = texture(texture1, coords);
}
