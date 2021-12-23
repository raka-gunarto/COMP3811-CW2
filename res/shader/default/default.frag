#version 330 core

out vec4 FragColor;

in vec3 cPos;
in vec3 normal;

uniform sampler2D texDiffuse;
uniform sampler2D texSpecular;

uniform int mode;
uniform vec3 cameraPos;
uniform vec3 color;

void main()
{
    // ambient
    float ambient = 0.5f;

    // TODO: texture if(mode == 1)
    FragColor = vec4(color, 1.0f) * (vec4(1.0f) * ambient);
    FragColor = vec4(1.0f);
}