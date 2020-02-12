#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aInstanceOffset;

out vec3 FragCoord;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

#define S(a, b, t) smoothstep(a, b, t)

void main()
{
    // gl_InstanceID (0-99 indexed instances)
    vec3 pos;
    pos = aPos * (( gl_InstanceID + 1) / 100.0 );

    pos + aInstanceOffset;

    // For fragment shader
    TexCoords = aTexCoords;
    Normal = aNormal;
    FragCoord = vec3(model * vec4(pos, 1.0));

    // output position
    gl_Position = projection * view * model * vec4(pos, 1.0);

}
