#version 330

//Zmienne jednorodne
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

//Atrybuty
in vec3 vertex; //wspolrzedne wierzcholka w przestrzeni modelu
in vec3 normal;
in vec2 texCoord0;

out vec2 iTexCoord0;
out vec3 normals;
out vec3 fragpos;

void main(void) {
    vec4 vert = vec4(vertex, 1);
    vec4 norm = vec4(normal, 0);

    gl_Position=P*V*M*vert;
    iTexCoord0 = texCoord0;
    normals=mat3(transpose(inverse(M))) * normal;
    fragpos = vec3(M*vert);
}
