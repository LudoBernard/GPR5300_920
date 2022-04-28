#version 330 core

layout (location = 0) out vec4 fragColor;
in vec3 ourColor;

uniform float colorCoeff;

void main() {
    fragColor = vec4(ourColor * colorCoeff, 1.0);
}