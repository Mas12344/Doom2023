#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_height1;


void main()
{    
    vec3 normal = texture(texture_normal1, TexCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0);
    vec4 h = texture(texture_height1, TexCoords);
    vec4 d = texture(texture_diffuse1, TexCoords);
    FragColor = normalize(h * d);
}