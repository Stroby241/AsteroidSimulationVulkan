#version 450
layout(early_fragment_tests) in;
layout(location = 0) out vec4 outColor;

layout(location = 0) in vec3 fragColor;



layout(set = 0,binding = 0) uniform GLobalUbo
{
   mat4 projectionView;
   vec2 cursorPos;
   vec2 windowSize;
   vec3 playerpos;
}ubo;

void main() {
    outColor = vec4(fragColor, 1.0); 
}