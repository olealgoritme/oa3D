#version 330 core

in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_texcoord;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
out vec4 fragColor;

void main() {
    fragColor = texture(texture_diffuse1, TexCoords);
}