#version 300 es
precision highp float;

layout (location = 0) out vec4 fragColor;

uniform vec3 lightColor;
uniform vec3 objectColor;

void main() {
    fragColor = vec4(lightColor * objectColor, 1.0);
}