#version 300 es

layout(location = 0) in vec3 aPosition;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform vec3 uColor;
uniform float uIntensity;

out vec3 vColor;

void main()
{
    gl_Position = uProjection * uView * uModel * vec4(aPosition, 1.0);
    vColor = uColor * uIntensity;
}