#version 330 core
layout (location = 0) in vec3 aPos;
uniform float radius;

void main() {
    gl_Position = vec4(aPos.x * radius, aPos.y* radius, aPos.z* radius, 1.0);
    gl_PointSize = 20.0;
}