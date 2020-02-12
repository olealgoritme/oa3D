#version 460 core

out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_ambient1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_reflection1;

void main() {

    vec3 diffuseMap = texture(texture_diffuse1, TexCoord).rgb;
    /*
     *vec3 ambientMap = texture(texture_ambient1, TexCoord).rgb;
     *vec3 specularMap = texture(texture_specular1, TexCoord).rgb;
     *vec3 reflectionMap = texture(texture_reflection1, TexCoord).rgb;
     */

    FragColor = vec4(diffuseMap, 1.0);
}
