#version 460 core
layout (location = 0) in vec3 aPos;

#define S(a, b, t) smoothstep(a,b,t)

uniform vec2 iResolution;
uniform float iTime;

in vec4 gl_FragCoord;
out vec4 fragColor;


float TaperBox(vec2 p, float wb, float wt, float yb, float yt, float blur) {
    float m = S(-blur, blur, p.y-yb);
    m *= S(blur, -blur, p.y-yt);

    p.x = abs(p.x);

    // 0 when p.y=yb and 1 when p.y=yt

    float w = mix(wb, wt, (p.y-yb)/ (yt-yb));
    m *= S(blur, -blur, p.x-w);
    return m;
}

vec4 Tree(vec2 uv, vec3 col, float blur) {

    float m = TaperBox(uv, .03, .03, -.05, .25, blur); // bottom
       m += TaperBox(uv, .15, .05, .25, .5, blur); // first tapered element
    m += TaperBox(uv, .15, .05, .5, .75, blur); // first tapered element
    m += TaperBox(uv, .1, .01, .75, 1., blur); // first tapered element

    float shadow = TaperBox(uv-vec2(0.2,0), .1, .5, .15, .25, blur);
    shadow += TaperBox(uv-vec2(0.25,0), .1, .5, .45, .5, blur);
    shadow += TaperBox(uv-vec2(0.25,0), .1, .5, .7, .75, blur);


    col -= shadow;
    return vec4(col, m);
}

float GetHeight(float x) {
return sin(x*.423)+sin(x)*.3;

}

vec4 Layer(vec2 uv, float blur) {
    // Fixed pixel color
    vec4 col = vec4(0);

    float id = floor(uv.x);
    float n = fract(sin(id*234.12)*5422.3)*2.-1.; // between 0-1 fractions
    float x = n*.3;
    float y = GetHeight(uv.x);
    float ground = S(blur, -blur, uv.y+y); // ground
    col += ground;

    y = GetHeight(id+.5+x);

    uv.x = fract(uv.x)-0.5;


    vec4 tree = Tree((uv-vec2(x, -y))*vec2(1,1.+n*.2), vec3(0.3), blur);
    //col.rg = uv;

    col = mix(col, tree, tree.a);
    col.a = max(ground, tree.a);
    return col;

}

float Hash21(vec2 p) {
    p = fract(p*vec2(234.25,754.34));
    p += dot(p, p+478.23);
    return fract(p.x*p.y);
}

vec4 TwinkleStarLayer(vec2 uv, float t) {
    vec4 col = vec4(0);
    // Create snow / stars / contrast pixels
    float twinkle = dot(length(sin(uv+t*3.)), length(cos(uv*vec2(22.,6.7)-t*3.)));
    //twinkle = sin(twinkle*10.)*.5+.5;

    float stars = pow(Hash21(uv), 1000.) * twinkle; // Functions that returns between 1 and 2 + add twinkle
    col += stars * 13.;

    // show twinkle in front
    //col += twinkle;

    return col;
}



void main() {
    // Normalized pixel coordinates (from 0 to 1)
    // vec2 uv = .5 /iResolution.xy;

    vec2 uv = (gl_FragCoord.xy-.5*iResolution.xy)/iResolution.xy;

    uv *= 10.;
    // (gl_FragCoord-.5*iResolution.xy)/iResolution.xy;
    //vec2 Mouse = (iMouse.xy / iResolution.xy)*2.-1.;

    //uv.x += iTime*.1;
    // zoom out
    //uv *=2.0;


    float t = iTime *.3;

    float blur = 0.005;

    // black
    vec4 col = vec4(0);

    // stars = first layer
    vec4 stars = vec4(TwinkleStarLayer(uv, t));
    col += stars;


    float moon = S(.35, .0003, length(uv-vec2(.15, .005)) -.05);
    //moon *= S(.3, .15, length(uv-vec2(.001,.001)));

    col += moon;

    vec4 layers;

    // Create several layers
    for(float i=0.; i < 1.; i+= 1./5.) {

        float scale = mix(30., 1., i);
        blur = mix(.09, .005, i);
        layers = Layer(uv*scale+vec2(t+i*100., i), blur);
        layers.rgb *= (1.-i)*vec3(.3,.7,.8);

        col = mix(col, layers, layers.a);
    }

    layers = Layer(uv+vec2(t*0.5, 1), .025);

    col = mix(col, layers*.1, layers.a);


    //  coordinate axis X + Y
    //  float thickness = 1./iResolution.y;
     //  if (abs(uv.x) < thickness) col.g = 1.;
     //  if (abs(uv.y) > thickness) col.r = 1.;


    // Output to screen
    fragColor = col;
}
