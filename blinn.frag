#version 400
in vec3 worldPos;
in vec3 vN;

out vec4 fragColor;

struct Light {
    vec3 position;
    vec3 color;
    float intensity;
    vec3 atten;     
};

#define MAX_LIGHTS 16
uniform Light uLight[MAX_LIGHTS];
uniform int   uNumLights;

uniform vec4 uColor;
uniform vec3 uCamPos;

const float kAmbient  = 0.10;
const float kSpecular = 0.30;
const float shininess = 60.0;

void main() {
    vec3 N = normalize(vN);
    vec3 V = normalize(uCamPos - worldPos);

    vec3 color = kAmbient * uColor.rgb;

    for (int i = 0; i < uNumLights; ++i) {
        vec3  Ldir = uLight[i].position - worldPos;
        float dist = length(Ldir);
        vec3  L    = (dist > 0.0) ? Ldir / dist : vec3(0.0);

        float NdotL = max(dot(N, L), 0.0);
        vec3  H     = normalize(L + V);
        float NdotH = max(dot(N, H), 0.0);
        float specPow = (NdotL > 0.0) ? pow(NdotH, shininess) : 0.0;

        float att = 1.0 / max(dot(uLight[i].atten, vec3(1.0, dist, dist*dist)), 1e-4);

        vec3 lightCol = uLight[i].color * uLight[i].intensity * att;

        vec3 diffuse  = uColor.rgb * NdotL * lightCol;
        vec3 specular = kSpecular * specPow * lightCol;

        color += diffuse + specular;
    }

    fragColor = vec4(color, uColor.a);
}
