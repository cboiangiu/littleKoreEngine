#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

layout(location = 0) out vec2 circlePosition;
layout(location = 1) out float radius;
layout(location = 2) out vec3 fragColor;

layout(binding = 0) uniform Data
{
    vec2 circlePosition;
    float radius;
    vec3 color;
}
data;

void main()
{
    gl_Position  = vec4(position, 1.0);
    circlePosition = data.circlePosition;
    radius = data.radius;
    fragColor = data.color;
}
