#version 460

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec2 iMouse;
uniform vec3 iResolution;
uniform float iTime;

void main() {
    vertex.time = iTime;
    vertex.position = vec4(projection * view * model * vec4(aPos, 1.0));
}