#version 450

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textCoords;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec4 aOffset; // pos.xyz w == index in texture array

out vec3 coords;
out vec3 FragPos;
out vec3 Normal;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

void main()
{
	FragPos = vec3(model * vec4(position + aOffset.xyz, 1.0));
	Normal = mat3(transpose(inverse(model))) * (aNormal+ aOffset.xyz);

	gl_Position = projection * view * vec4(position + aOffset.xyz, 1.0f);
	coords = vec3(textCoords,aOffset.w);
}
