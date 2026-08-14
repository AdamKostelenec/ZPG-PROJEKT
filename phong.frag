#version 400

in vec3 worldPos;
in vec3 vN;
in vec2 vUV;

out vec4 fragColor;


struct Light {
    vec3 position;
    vec3 direction;
    vec3 color;
    float intensity;
    float cutoff;
    vec3 atten;   
    int  type;    // 0 = directional, 1 = point, 2 = spot, 3 = ambient
};

#define MAX_LIGHTS 16
uniform Light uLight[MAX_LIGHTS];
uniform int   uNumLights;

struct Material {
    float ra;   // ambient
    float rd;   // diffuse
    float rs;   // specular
    float h;    // shininess
};
uniform Material uMat;

uniform vec4      uColor;      
uniform sampler2D uTex;       
uniform bool      uHasTex;    

uniform vec3  uCamPos;         

// 0 = constant, 1 = lambert, 2 = phong, 3 = blinn
uniform int uShadingMode;


vec3 getAlbedo()
{
    vec3 albedo = uColor.rgb;
    if (uHasTex) {
        vec4 texel = texture(uTex, vUV);
        albedo *= texel.rgb;
    }
    return albedo;
}


bool computeLightFactors(
    in Light Lsrc,
    in vec3  P,
    out vec3 Ldir,
    out float NdotL,
    in vec3 N,
    out float att,
    out float spotF)
{
    if (Lsrc.type == 3) { //ambient
        att   = 1.0;
        spotF = 1.0;
        NdotL = 0.0;
        Ldir  = vec3(0.0);
        return false;
    }

    if (Lsrc.type == 0) {  //directional
        Ldir = normalize(-Lsrc.direction);
        att = 1.0;
    } else {
        vec3 toLight = Lsrc.position - P;
        float dist = length(toLight);
        if (dist > 0.0)
            Ldir = normalize(toLight);
        else
           Ldir = vec3(0.0);


        float denom = dot(Lsrc.atten, vec3(1.0, dist, dist * dist));
        att = 1.0 / max(denom, 1e-4);
    }


    NdotL = max(dot(N, Ldir),0.0);
    if (NdotL <= 0.0)
       return false;    

    // spotlight
    spotF = 1.0;
    if (Lsrc.type == 2) {
        vec3 spotDir = normalize(-Lsrc.direction);  
        float cosTheta = dot(Ldir, spotDir);

        float cosOuter = Lsrc.cutoff;
        float cosInner = mix(cosOuter, 1.0, 0.3);  

        spotF = smoothstep(cosOuter, cosInner, cosTheta);
    }

    return true;
}


void main()
{
    vec3 N = normalize(vN);
    vec3 V = normalize(uCamPos - worldPos);
    vec3 albedo = getAlbedo();

    if (uShadingMode == 0) {
        fragColor = vec4(albedo, uColor.a);
        return;
    }

    vec3 color = vec3(0.0);
    vec3 ambientAccum = vec3(0.0);

    for (int i = 0; i < uNumLights && i < MAX_LIGHTS; ++i) {
        Light Lsrc = uLight[i];

        if (Lsrc.type == 3) {
            ambientAccum += uMat.ra * albedo * Lsrc.color * Lsrc.intensity;
            continue;
        }

        vec3 Ldir;
        float NdotL;
        float att, spotF;

        if (!computeLightFactors(Lsrc, worldPos, Ldir, NdotL, N, att, spotF))
            continue;

        vec3 lightCol = Lsrc.color * Lsrc.intensity * att * spotF;

    
        vec3 diffuse = uMat.rd * albedo * NdotL * lightCol;

        float specPw = 0.0;
        if (uShadingMode == 2 || uShadingMode == 3) {
            if (uMat.rs > 0.0) {
                if (uShadingMode == 2) {
                    vec3 R = reflect(-Ldir, N);
                    float RdotV = max(dot(R, V), 0.0);
                    specPw = pow(RdotV, uMat.h);
                } else {
                    vec3 H = normalize(Ldir + V);
                    float NdotH = max(dot(N, H), 0.0);
                    specPw = pow(NdotH, uMat.h);
                }
            }
        }
        vec3 specular = uMat.rs * specPw * lightCol;

        color += diffuse + specular;
    }

    color += ambientAccum;

    fragColor = vec4(color, uColor.a);
}
