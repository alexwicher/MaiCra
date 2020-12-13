#version 450

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec3 coords;
    vec4 FragPosLightSpace;
} fs_in;
out vec4 outputColor;

struct Light {
//vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform sampler2DArray texture1;
uniform sampler2D shadowMap;
uniform vec3 viewPos;
uniform Light light;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float bias = max(0.05 * (1.0 - dot(fs_in.Normal, light.direction)), 0.005);

    // PCF: percentage-closer filtering
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    const int halfkernelWidth = 2;
    for(int x = -halfkernelWidth; x <= halfkernelWidth; ++x)
    {
        for(int y = -halfkernelWidth; y <= halfkernelWidth; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= ((halfkernelWidth*2+1)*(halfkernelWidth*2+1));

    if(projCoords.z > 1.0)
    shadow = 0.0;

    return shadow;
}

void main()
{
    vec3 lightColor = vec3(0.3);
    vec3 normal = normalize(fs_in.Normal - fs_in.FragPos);
    vec4 textureColor = texture(texture1, fs_in.coords);

    // ambient
    vec3 ambient = light.ambient * textureColor.rgb;

    // diffuse
    vec3 norm = normalize(fs_in.Normal);

    vec3 lightDir = normalize(light.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light.diffuse  * textureColor.rgb;

    // specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;

    // calculate shadow
    float shadow = ShadowCalculation(fs_in.FragPosLightSpace);
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse +specular)) * textureColor.rgb;

    outputColor = vec4(lighting, textureColor.a);

}
