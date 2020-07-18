#version 130

in vec3 position;
in vec3 textCoords;

out vec3 coords;

uniform mat4 modelView;
uniform mat4 projection;

void main()
{
	gl_Position = projection * modelView * vec4(position, 1.0f);
	coords = textCoords;
}
