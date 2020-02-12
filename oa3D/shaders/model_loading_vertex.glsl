#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBiTangent;

out vec3 FragCoord;
out vec3 Normal;
out vec2 TexCoords;
out vec3 Tangent;
out vec3 BiTangent;
out vec3 Position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec4 pos = vec4(aPos, 1.0);
    mat4 mvp = projection * view * model;
    gl_Position = mvp * pos;

    //pos *= mix(vec4(-.3), vec4(-.1), vec4(-.005));
    // For fragment shader
    FragCoord = vec3(mvp * pos);
    Normal = aNormal; //mat3(transpose(inverse(model))) * aNormal;
    Position = vec3(model * pos);
    TexCoords = aTexCoords;
    Tangent = aTangent;
    BiTangent = aBiTangent;
}
