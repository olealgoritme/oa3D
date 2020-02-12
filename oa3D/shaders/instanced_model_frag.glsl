#version 460 core
// fragment output color
out vec4 FragColor;

// From vertex shader
in vec2 TexCoords;
in vec3 Normal;
in vec3 FragCoord;

// 2d texture sampling
uniform sampler2D texture_diffuse1;

// generic uniform values for more manipulation later
uniform vec2  iMouse;
uniform float iTime;
uniform vec3  iResolution;

// light
uniform vec3 lightPos;
uniform vec3 viewPos;

#define S(a, b, t) smoothstep(a, b, t)

void main()
{
    vec4 tex = texture2D(texture_diffuse1, TexCoords);
    // simple color component tinting
    //FragColor = tex + vec4(0.5, 0, 0, 1)*tex.a;

    // standard colorization
    //FragColor = texture2D(texture_diffuse1, TexCoords);

    // Varying gradient color over time
    vec2 uv = vec2( ((FragCoord.xy - 0.5) * iResolution.xy) / iResolution.xy);
    float gradient = uv.y;
    vec3 col = 0.2 + 0.2 * cos(iTime+uv.xyx+vec3(0,2,4));
    vec3 base = sin( ( iTime * .25) * vec3( .345, .456, .43 ))*.2 + .5;
    col += vec3( 0. * base);
    col += gradient + S(.35, .55, base) * .35;

    vec3 lightColor = vec3(0., 1., 0.);
    vec3 objectColor = col;

    // normalized coords
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragCoord);

    // ambient
    float ambientStrength = 1.0;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular
    float specularStrength = 5.0;
    vec3 viewDir = normalize(viewPos - FragCoord);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);
    vec3 specular = specularStrength * spec * lightColor;

    // Adding ambient + diffuse + specular lighting = phong lighting
    vec3 phong = (ambient + diffuse + specular) * objectColor;

    // adding phong
    col+= phong;

    // Output color
    FragColor = tex + vec4(col, 1.0)*tex.a;


}
