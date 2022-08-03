#version 450

precision highp float;

layout(location = 0) in vec2 circlePosition;
layout(location = 1) in float radius;
layout(location = 2) in vec3 fragColor;
layout(location = 3) in vec2 windowSize;

layout(location = 0) out vec4 outColor;

void main()
{    
    float circleRadius = min(circlePosition.x, circlePosition.y) / radius;

    float thickness = 1.;
    float fade = 4 / windowSize.y * circleRadius;

    // -1 -> 1 local space, adjusted for aspect ratio
    vec2 uv = (gl_FragCoord.xy - circlePosition) / min(circlePosition.x, circlePosition.y) * circleRadius;

    // Calculate distance and fill circle with white
    float distance = 1.0 - length(uv);
    vec4 color = vec4(smoothstep(0.0, fade, distance));
    color *= vec4(smoothstep(thickness + fade, thickness, distance));

    // Set output color
    outColor = vec4(color);
    outColor.rgb *= fragColor;
    outColor.a *= 0.9;
}
