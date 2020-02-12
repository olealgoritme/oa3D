#version 330 core

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_color;
layout (location = 2) in vec2 vertex_texcoord;

out vec3 vs_position;
out vec3 vs_color;
out vec2 vs_texcoord;

out vec2 TexCoords;

uniform vec2 iMouse;
uniform vec3 iResolution;
uniform float iTime;

void main()
{

    TexCoords = vertex_texcoord;
    vs_position = vertex_position;
    vs_color = vertex_color;
    vs_texcoord = vertex_texcoord;

    gl_Position = vec4(vertex_position, 1.0f);
}