#version 330 core

layout(location = 0) in vec3 aPos;

out vec3 vTexCoord;

uniform mat4 uView;
uniform mat4 uProj;

void main()
{
    vec4 pos = uProj * uView * vec4(aPos, 1.0);
    gl_Position = vec4(pos.xy, pos.w, pos.w);
    vTexCoord = aPos;
}
