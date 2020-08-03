#version 450

in vec3 coords;
in vec3 Normal;
in vec3 FragPos;
out vec4 outputColor;

struct Light {
//vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

uniform sampler2DArray texture1;
uniform vec3 viewPos;
uniform Light light;

void main()
{

    vec4 textureColor = texture(texture1, coords);

    // ambient
    vec3 ambient = light.ambient * textureColor.rgb;

    // diffuse
    vec3 norm = normalize(Normal);

    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light.diffuse  * textureColor.rgb;

    //	// specular
    //	vec3 viewDir = normalize(viewPos - FragPos);
    //	vec3 reflectDir = reflect(-lightDir, norm);
    //	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    //	vec3 specular = light.specular * spec  * textureColor.rgb;

//    float distance    = length(-light.direction - FragPos);
//    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
//
//    ambient  *= attenuation;
//    diffuse   *= attenuation;
////    specular *= attenuation;


    outputColor = vec4((ambient+diffuse+specular), textureColor.a);

}
