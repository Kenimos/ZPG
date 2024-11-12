// blinn_fragment.glsl
#version 330 core

in vec3 fragPosition;
in vec3 fragNormal;

out vec4 outColor;

struct Light {
    vec3 position;
    vec3 color;
    float intensity;
};

#define MAX_LIGHTS 10
uniform int numLights;
uniform Light lights[MAX_LIGHTS];

uniform vec3 viewPos;
uniform vec3 materialColor;

void main()
{
    vec3 norm = normalize(fragNormal);
    vec3 viewDir = normalize(viewPos - fragPosition);

    vec3 ambient = vec3(0.0);
    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);

    float ambientStrength = 0.1;
    float specularStrength = 0.5;

    float constant = 1.0;    
    float linear = 0.09;   
    float quadratic = 0.032;

    for (int i = 0; i < numLights; ++i)
    {
        Light light = lights[i];

        vec3 ambientComponent = ambientStrength * light.color * light.intensity;
        ambient += ambientComponent;

        vec3 lightDir = normalize(light.position - fragPosition);
        float diff = max(dot(norm, lightDir), 0.0);

        if (diff > 0.0)
        {
            float distance = length(light.position - fragPosition);
            float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));
            vec3 diffuseComponent = diff * light.color * light.intensity * attenuation;
            diffuse += diffuseComponent;

            vec3 halfwayDir = normalize(lightDir + viewDir);
            float spec = pow(max(dot(norm, halfwayDir), 0.0), 32.0);
            vec3 specularComponent = specularStrength * spec * light.color * light.intensity * attenuation;
            specular += specularComponent;
        }
    }

    vec3 result = (ambient + diffuse + specular) * materialColor;
    outColor = vec4(result, 1.0);
}
