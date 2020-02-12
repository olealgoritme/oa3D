#version 460 core

out vec4 FragColor;
in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{

     FragColor = texture(skybox, TexCoords);

     // Material Refractive index
    // Air      1.00
    // Water    1.33
    // Ice      1.309
    // Glass    1.52
    // Diamond  2.42

    /*
     *float ratio = 1.00 / 1.52;
     *vec3 I = normalize(Position - cameraPos);
     *vec3 R = reflect(I, normalize(Normal), ratio);
     *FragColor = vec4(texture(skybox, R).rgb, 1.0);
     */
}
