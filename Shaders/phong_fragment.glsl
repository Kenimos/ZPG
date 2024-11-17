#version 330 core

in vec3 fragPosition;
in vec3 fragNormal;

out vec4 outColor;

struct Light {
    int type; // 0: Point light, 1: Directional light, 2: Spotlight
    vec3 position;
    vec3 direction;
    vec3 color;
    float intensity;
    float innerCutOff;
    float outerCutOff;
};

// Material properties
uniform vec3 materialColor;
uniform float materialShininess;
uniform float ambientStrength;
uniform float specularStrength; // Added missing semicolon

#define MAX_LIGHTS 10
uniform int numLights;
uniform Light lights[MAX_LIGHTS];

uniform vec3 viewPos;

void main()
{
    vec3 norm = normalize(fragNormal);
    vec3 viewDir = normalize(viewPos - fragPosition);

    vec3 ambient = vec3(0.0);
    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);

    for (int i = 0; i < numLights; ++i)
    {
        Light light = lights[i];

        vec3 lightDir;
        float attenuation = 1.0;

        if (light.type == 0) // Point light
        {
            lightDir = normalize(light.position - fragPosition);
            float distance = length(light.position - fragPosition);
            attenuation = 1.0 / (distance * distance);
        }
        else if (light.type == 1) // Directional light
        {
            lightDir = normalize(-light.direction);
        }
        else if (light.type == 2) // Spotlight
        {
            lightDir = normalize(light.position - fragPosition);
            float theta = dot(lightDir, normalize(-light.direction));
            float epsilon = light.innerCutOff - light.outerCutOff;
            float intensitySpot = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

            attenuation = intensitySpot;

            // Optional: Add distance attenuation for spotlight
            float distance = length(light.position - fragPosition);
            attenuation *= 1.0 / (distance * distance);
        }

        // Ambient component
        vec3 ambientComponent = ambientStrength * light.color * light.intensity * attenuation;
        ambient += ambientComponent;

        // Diffuse component
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuseComponent = diff * light.color * light.intensity * attenuation;
        diffuse += diffuseComponent;

        // Specular component
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialShininess);
        vec3 specularComponent = specularStrength * spec * light.color * light.intensity * attenuation;
        specular += specularComponent;
    }

    vec3 result = (ambient + diffuse + specular) * materialColor;
    outColor = vec4(result, 1.0);
}
