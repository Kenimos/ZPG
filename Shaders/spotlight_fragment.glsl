#version 330 core

struct Light {
    vec3 position;
    vec3 color;
    float intensity;
    vec3 direction;    // Direction the spotlight is pointing
    float cutoff;      // Cosine of the cutoff angle
};

uniform int numLights;
uniform Light lights[10]; // MAX_LIGHTS = 10

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 viewPos;
uniform vec3 materialColor;

void main()
{
    vec3 ambient = 0.1 * materialColor;
    vec3 result = ambient;

    for(int i = 0; i < numLights && i < 10; ++i)
    {
        // Diffuse
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(lights[i].position - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lights[i].color * lights[i].intensity;

        // Specular
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        vec3 specular = spec * lights[i].color * lights[i].intensity;

        // Spotlight (Flashlight)
        float theta = dot(lightDir, normalize(-lights[i].direction));
        if(theta > lights[i].cutoff) // Inside the spotlight cone
        {
            float epsilon = 0.1; // Soft edge
            float intensityFactor = clamp((theta - lights[i].cutoff) / epsilon, 0.0, 1.0);
            diffuse *= intensityFactor;
            specular *= intensityFactor;
            result += diffuse + specular;
        }
    }

    FragColor = vec4(result, 1.0);
}
