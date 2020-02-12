#version 460 core

#define S(a, b, t) smoothstep(a, b, t)

// fragment output color
out vec4 FragColor;

// From vertex shader
in vec3 FragCoord;
in vec3 Normal;
in vec2 TexCoords;
in vec3 Tangent;
in vec3 BiTangent;
in vec3 Position;


// uniform inputs from CPU
// model samples
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_ambient1;
uniform sampler2D texture_reflection1;

uniform vec2      iMouse;
uniform float     iTime;
uniform vec2      iResolution;
uniform vec3      cameraPos;

// lights
uniform vec3      lightPos;
uniform vec3      viewPos;


vec3 getlights(vec3 color)
{
    // Adding ambient + diffuse + specular lighting = phong lighting
    vec3 lightColor = vec3(0.2, 0.5, 1.);

    // normalized coords
    vec3 lightP = vec3(0.5, 0.5, 0.5);


    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightP - FragCoord);

    // ambient
    float ambientStrength = 1.0;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular
    float specularStrength = 1.0;
    vec3 viewDir = normalize(viewPos - FragCoord);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);
    vec3 specular = specularStrength * spec * lightColor;

    // adding phong
    vec3 phong = (ambient + diffuse + specular) * color;
    return phong;
}


vec3 lights()
{
    vec3 color = vec3(0.3, 0.5, 0.7);
    vec3 diffuse = texture(texture_diffuse1, TexCoords).rgb;
    vec3 ambient = texture(texture_ambient1, TexCoords).rgb;
    vec3 specular = texture(texture_specular1, TexCoords).rgb;
    vec3 reflection = texture(texture_reflection1, TexCoords).rgb;
    return (diffuse + ambient + specular + reflection) * color;
}

void main()
{

    // Varying gradient color over time
    vec2 uv = vec2( ((FragCoord.xy - 0.5) * iResolution.xy) / iResolution.xy);
    vec4 tex = texture2D(texture_diffuse1, TexCoords);

    vec3 col = 0.5 + 0.25 * cos(iTime+uv.xyx+vec3(0,2,4));
    vec3 base = sin( ( iTime * .15) * vec3( .11, .456, .43 ))*.2 + .5;
    vec3 lighting = getlights(vec3(0.1, 0.5, 0.3));
    float gradient = uv.y;
    col += vec3( 0.05 * base);
    col += gradient + S(.35, .55, base) * .35;

    FragColor = tex + vec4(col, 1.0) * tex.a;

}
