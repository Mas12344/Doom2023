#version 330

uniform sampler2D diffuseTexture;

out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela
in vec2 iTexCoord0;

uniform float dissolve;
uniform float ior;
uniform float shininess;

void main(void) {
  
    vec4 col = texture(diffuseTexture, iTexCoord0);

    pixelColor = col;


}
