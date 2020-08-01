#version 450

in vec3 coords;
in vec3 Normal;
in vec3 FragPos;

out vec4 outputColor;

uniform sampler2DArray texture1;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

void main()
{

	vec4 textureColor = texture(texture1, coords);

	// ambient
	float ambientStrength = 0.1f;
	vec3 ambient = ambientStrength * lightColor;

	// diffuse
	vec3 norm = normalize(Normal);

	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	// specular
	float specularStrength = 0.25f;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

	vec3 specular = specularStrength * lightColor * spec;


	outputColor = vec4((ambient+diffuse+specular) * textureColor.rgb,textureColor.a);
}
