#version 400
in vec3 vN;
in vec2 vUV;                 

uniform vec4 uColor;
uniform sampler2D uTex;      
uniform bool uHasTex;       

out vec4 fragColor;

void main() {
    float t = clamp(normalize(vN).y * 0.5 + 0.5, 0.0, 1.0);
    vec3 shade = mix(vec3(0.6, 0.6, 0.6), vec3(1.0, 1.0, 1.0), t);

    vec4 base = vec4(uColor.rgb * shade, uColor.a);

    if (uHasTex) {
        base *= texture(uTex, vUV);
    }

    fragColor = base;
}
