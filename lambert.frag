#version 400
in vec3 worldPos;
in vec3 vN;

out vec4 fragColor;

struct Light { vec3 position; vec3 color; float intensity; };
uniform Light uLight;
uniform vec4  uColor;

void main() {
    vec3 N = normalize(vN);
    vec3 L = normalize(uLight.position - worldPos);
    float NdotL = max(dot(N, L), 0.0);

    vec3 ambient = 0.10 * uColor.rgb;
    vec3 diffuse = uColor.rgb * NdotL * uLight.color * uLight.intensity;
    fragColor = vec4(ambient + diffuse, uColor.a);
}
