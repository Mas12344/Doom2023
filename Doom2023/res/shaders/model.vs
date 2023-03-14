#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitTangent;
layout (location = 5) in vec4 aID;
layout (location = 6) in vec4 aWeights;

out vec2 TexCoords;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

void main()
{
    TexCoords = aTexCoords;    
    gl_Position = P * V * M * vec4(aPos, 1.0);
}