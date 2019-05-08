#version 330
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_separate_shader_objects : enable

// output written to the screen is always location 0
layout(location = 0) out vec3 vColor;

layout(location = 0) in vec2 inPos;
layout(location = 1) in vec3 aColor;

void main() {
        vColor = aColor; // Gib die Farbe des Vertexes an den Fragment Shader weiter..
        gl_Position = vec4(inPos, 0.0f, 1.0f);
}