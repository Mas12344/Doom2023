#version 330

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;

out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela
in vec2 iTexCoord0;
in vec3 normals;
in vec3 fragpos;

uniform vec3 lights[40];
uniform vec3 viewPos;
uniform vec3 cameradir;

uniform float dissolve;
uniform float ior;
uniform float shininess = 32.0;

vec3 CalcPointLight(vec3 lightpos, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(lightpos - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // attenuation
    float distance    = length(lightpos - fragPos);
    float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * (distance * distance)); 
    // combine results
    vec3 ambient  = vec3(0.0, 0.02, 0.025)  * vec3(texture(diffuseTexture, iTexCoord0));
    vec3 diffuse  = vec3(0.0, 0.26953125, 0.70703125)  * diff * vec3(texture(diffuseTexture, iTexCoord0));
    vec3 specular = vec3(0.546875, 0.8984375, 0.9765625) * spec * vec3(texture(specularTexture, iTexCoord0));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
} 

vec3 CalcDirLight(vec3 lightdir, vec3 normal, vec3 viewDir)
{

    vec3 lightDir = normalize(-lightdir);
    float lightcutOff = 0.976;
    float theta = dot(viewDir, lightDir);
    float epsilon = lightcutOff - 0.956;
    float intensity = clamp((theta - 0.956) / epsilon, 0.0, 1.0);
    
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // combine results
    vec3 ambient  = vec3(0.05, 0.05, 0.05)  * vec3(texture(diffuseTexture, iTexCoord0));
    vec3 diffuse  = intensity * vec3(0.4, 0.4, 0.4)  * diff * vec3(texture(diffuseTexture, iTexCoord0));
    vec3 specular = intensity * vec3(0.5, 0.5, 0.5) * spec * vec3(texture(specularTexture, iTexCoord0));
    return (ambient + diffuse + specular);
    

}  

void main(void) {
  
    vec3 col = vec3(0);
    vec3 viewdir = normalize(viewPos-fragpos);
    col += CalcDirLight(cameradir, normalize(normals), viewdir);

    for(int i = 0; i < 40; i++){
        col += 0.8 * CalcPointLight(lights[i], normalize(normals), fragpos, viewdir);
    }
    pixelColor = vec4(col,1.0);


}
