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

uniform float time;

void main()
{
    // material diffuse and specular color (branchless)
    vec3 dColor = diffuseColor + texture(diffuseTex, texCoords).rgb;
    FragColor = vec4(mix(dColor, vec3(1.0,1.0,1.0), sin(time * 10)), 1.0);
}