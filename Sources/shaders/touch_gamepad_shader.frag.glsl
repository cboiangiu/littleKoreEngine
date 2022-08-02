#version 450

precision highp float;

layout(location = 0) in vec2 circlePosition;
layout(location = 1) in float radius;
layout(location = 2) in vec3 fragColor;

layout(location = 0) out vec4 outColor;

void main()
{
    float dist = distance(circlePosition, gl_FragCoord.xy);
    if(dist == 0.)
        dist = 1.;
    if(radius / dist >= 1.)
        outColor = vec4(fragColor, 1.0);
    else{
        discard;
    }
}
