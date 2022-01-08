#version 330 core

out vec4 FragColor;

in vec2 texCoords;

in vec3 cPos;
in vec3 normal;

uniform sampler2D diffuseTex;
uniform sampler2D specularTex;
uniform vec3 diffuseColor;
uniform vec3 specularColor;
uniform float shininess;

// fog parameters
in float depth; 
uniform vec3 backgroundColor; 
uniform float farPlane;
uniform float fogOffset;

uniform vec3 ambientColor;
uniform float ambientIntensity;

uniform vec3 cameraPos;

struct PointLight {
    vec3 pos;

    float linAttenuate;
    float quadAttenuate;

    vec3 color;
};
struct DirectionalLight {
    vec3 dir;
    vec3 color;
};
uniform PointLight lights[16];
uniform DirectionalLight sun;
uniform sampler2DShadow sunShadow;
in vec4 sunSpacePos;

void main()
{
    // material diffuse and specular color (branchless)
    vec3 dColor = diffuseColor + texture(diffuseTex, texCoords).rgb;
    vec3 sColor = specularColor + texture(specularTex, texCoords).rgb;

    vec3 ambient = ambientColor * ambientIntensity * dColor;
    vec3 finalColor = ambient;
    vec3 viewDirection = normalize(cameraPos - cPos);

    // calculate all point lights
    for (int i = 0; i < 16; i++)
    {
        vec3 lightDirection = normalize(lights[i].pos - cPos);

        // calculate diffuse and specular coefficients
        float diffuseCoefficient = max(dot(normal, lightDirection), 0);
        float specularCoefficient = pow(max(dot(viewDirection, reflect(-lightDirection, normal)), 0),shininess);

        // calculate diffuse and specular lighting
        vec3 diffuse = lights[i].color * diffuseCoefficient * dColor;
        vec3 specular = lights[i].color * specularCoefficient * sColor;

        // attenuation (combine linear and quadratic branchless)
        float dist = length(lights[i].pos - cPos);
        float attenuation = 1.0 / (1 + lights[i].linAttenuate * dist + lights[i].quadAttenuate * pow(dist, 2));
        diffuse *= attenuation;
        specular *= attenuation;
        
        // combine into finalColor
        finalColor += diffuse + specular;
    }

    // calculate the one directional light (sun)
    if (vec3(sun.color) != vec3(0.0f, 0.0f, 0.0f))
    {
        // calculate diffuse + specular
        vec3 sunDirection = normalize(-sun.dir);
        float sunDiffuseCoefficient = max(dot(normal, sunDirection), 0);
        float sunSpecularCoefficient = pow(max(dot(viewDirection, reflect(-sunDirection, normal)), 0),shininess);
        vec3 sunDiffuse = sun.color * sunDiffuseCoefficient * dColor;
        vec3 sunSpecular = sun.color * sunSpecularCoefficient * sColor;

        // calculate shadows
        vec3 sunProjection = sunSpacePos.xyz / sunSpacePos.w; // perspective divide
        sunProjection = sunProjection * 0.5 + 0.5;
        float bias = max(0.001 * (1.0 - dot(normal, sunDirection)), 0.001);
        float shadowCoefficient = texture(sunShadow, vec3(sunProjection.xy, sunProjection.z - bias));

        finalColor += shadowCoefficient * (sunDiffuse + sunSpecular);
    }

    // "fog"
    finalColor = mix(finalColor, backgroundColor, smoothstep(farPlane-fogOffset,farPlane,depth));

    FragColor = vec4(finalColor, 1.0);
}