#version 400

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUV;

out vec3 worldPos;
out vec3 vN;
out vec2 vUV;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

uniform mat4 uCustom;   
uniform float w = 1.0;

void main()
{
    vec4 localPos = vec4(aPos, w);

    vec4 wp = uCustom * (uModel * localPos);

    worldPos = wp.xyz;

    vN = mat3(transpose(inverse(uModel))) * aNormal;

    vUV = aUV;

    gl_Position = uProj * uView * wp;
}
