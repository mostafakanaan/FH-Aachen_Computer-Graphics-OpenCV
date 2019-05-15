#version 330
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 vColor;

// output written to the screen is always location 0
layout(location = 0) out vec4 fragColor;

uniform float uAlpha;

void main(void)
{
    fragColor = vec4(vColor,uAlpha);
}
