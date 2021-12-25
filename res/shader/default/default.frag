#version 330 core

out vec4 FragColor;

in vec2 texCoords;

in vec3 cPos;
in vec3 normal;

uniform sampler2D texDiffuse;
uniform sampler2D texSpecular;
uniform vec3 diffuseColor;
uniform float specularIntensity;

uniform vec3 ambientColor;
uniform float ambientIntensity;

uniform vec3 cameraPos;

void main()
{
    vec3 dColor = diffuseColor + texture(texDiffuse, texCoords).rgb;
    vec3 ambient = ambientColor * ambientIntensity * dColor;
    vec3 diffuse = dColor * 0.0;

    vec3 finalColor = ambient + diffuse;
    FragColor = vec4(finalColor, 1.0);
}