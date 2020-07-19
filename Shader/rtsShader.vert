#version 450

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textCoords;
layout (location = 2) in vec4 aOffset; // pos.xyz w == index in texture array

out vec3 coords;

uniform mat4 modelView;
uniform mat4 projection;

void main()
{
	gl_Position = projection * modelView * vec4(position + aOffset.xyz, 1.0f);
	coords = vec3(textCoords,aOffset.w);
}
