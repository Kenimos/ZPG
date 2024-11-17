// spotlight_fragment.glsl
#version 330 core

struct Light {
    int type; // 0: Point light, 1: Directional light, 2: Spotlight
    vec3 position;
    vec3 direction;
    vec3 color;
    float intensity;
    float innerCutOff;
    float outerCutOff;
};

#define MAX_LIGHTS 10
uniform int numLights;
uniform Light lights[MAX_LIGHTS];

uniform float ambientStrength;

in vec3 FragPos;
in vec3 Normal;

uniform vec3 viewPos;

uniform vec3 materialColor;
uniform float materialShininess;

out vec4 FragColor;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result = vec3(0.0f);

    for(int i = 0; i < numLights; ++i)
    {
        vec3 ambient = ambientStrength * lights[i].color * lights[i].intensity;
        vec3 lightDir;
        float attenuation = 1.0;

        if (lights[i].type == 0) // Point light
        {
            lightDir = normalize(lights[i].position - FragPos);
            float distance = length(lights[i].position - FragPos);
            attenuation = 1.0 / (distance * distance);
        }
        else if (lights[i].type == 1) // Directional light
        {
            lightDir = normalize(-lights[i].direction);
        }
        else if (lights[i].type == 2) // Spotlight
        {
            lightDir = normalize(lights[i].position - FragPos);
            float theta = dot(lightDir, normalize(-lights[i].direction));
            float epsilon = lights[i].innerCutOff - lights[i].outerCutOff;
            float intensitySpot = clamp((theta - lights[i].outerCutOff) / epsilon, 0.0, 1.0);

            attenuation = intensitySpot;

            // Optional: Add distance attenuation for spotlight
            float distance = length(lights[i].position - FragPos);
            attenuation *= 1.0 / (distance * distance);
        }

        // Diffuse shading
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lights[i].color * lights[i].intensity;

        // Specular shading
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialShininess);
        vec3 specular = spec * lights[i].color * lights[i].intensity;

        diffuse *= attenuation;
        specular *= attenuation;
        ambient *= attenuation;

        result += (ambient + diffuse + specular);
    }

    FragColor = vec4(result * materialColor, 1.0f);
}
