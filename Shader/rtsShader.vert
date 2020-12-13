#version 450

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textCoords;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec4 aOffset; // pos.xyz w == index in texture array

out VS_OUT {
	vec3 FragPos;
	vec3 Normal;
	vec3 coords;
	vec4 FragPosLightSpace;
} vs_out;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;
uniform mat4 lightSpaceMatrix;

void main()
{
	vs_out.FragPos = vec3(position + aOffset.xyz);

	vs_out.Normal = (aNormal);
	vs_out.coords = vec3(textCoords,aOffset.w);
	vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);

	gl_Position = projection * view * vec4(vs_out.FragPos, 1.0f);
}
