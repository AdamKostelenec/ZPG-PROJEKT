#version 400
layout(location = 0) in vec3 localPosition;
layout(location = 1) in vec3 localNormal;
layout(location = 2) in vec2 localUV;      

uniform mat4 modelMatrix;

out vec3 vN;   
out vec2 vUV; 

void main(void)
{
    mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));
    vN = normalize(normalMatrix * localNormal);

    vUV = localUV;

    gl_Position = modelMatrix * vec4(localPosition, 1.0);
}
