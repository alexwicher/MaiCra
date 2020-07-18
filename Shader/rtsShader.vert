#version 130

in vec3 position;
in vec2 textCoords;
in vec4 aOffset; // pos.xyz w == index in texture array

out vec3 coords;

uniform mat4 modelView;
uniform mat4 projection;

void main()
{
	gl_Position = projection * modelView * vec4(position + aOffset.xyz, 1.0f);
	coords = vec3(textCoords,aOffset.w);
}
