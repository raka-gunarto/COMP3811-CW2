#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 texCoords;

out vec3 cPos;
out vec4 sunSpacePos;
out vec3 normal;

out float depth;

uniform mat4 cameraMat;
uniform mat4 model;
uniform mat4 view;
uniform mat4 sunViewProjection;
uniform mat3 normalMat;

uniform float time;

void main()
{
    cPos = vec3(model * vec4(vec3(aPos.x *(sin(time*5) * 0.5 + 1.5)*(aPos.y * 0.02),aPos.yz), 1.0f));
    sunSpacePos = sunViewProjection * vec4(cPos, 1.0f);
    gl_Position = cameraMat * vec4(cPos, 1.0f);
    normal = normalize(normalMat * aNormal);
    depth = -(view * vec4(cPos, 1.0f)).z;
    texCoords = aTexCoords;
}