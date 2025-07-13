#version 300 es
precision mediump float;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
out vec3 vColor;

void main() {
    vColor = aColor;
    gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
} 