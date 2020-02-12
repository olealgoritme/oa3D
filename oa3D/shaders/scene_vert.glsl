#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 view;
uniform mat4 projection;

void main() {
    vec4 pos = projection * view * vec4(aPos, 1.0);
    gl_Position = pos;
    TexCoord = aTexCoord;
}
