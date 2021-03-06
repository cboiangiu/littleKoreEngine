#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

layout(location = 0) out vec3 fragColor;

layout(set = 0, binding = 0) uniform Model
{
    mat4 transform;
}
model;

void main()
{
    gl_Position = model.transform * vec4(position, 1.0);
    fragColor = color;
}
