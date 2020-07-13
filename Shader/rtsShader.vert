#version 130

in vec3 position;
in vec2 textCoords;

out vec2 coords;

//uniform mat4 view;
//uniform mat4 projection;
//uniform mat4 model;

void main()
{
//	projection * view * model *
	gl_Position = vec4(position, 1.0f);
	coords = textCoords;
}
