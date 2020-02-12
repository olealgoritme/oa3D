#version 460 core
layout (location = 0) in vec3 aPos;

out vec4 FragCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec4 pos = vec4(aPos, 1.0);
    mat4 mvp = projection * view * model;
    gl_Position = mvp * pos;
    FragCoord = vec4(mvp * pos);
}
