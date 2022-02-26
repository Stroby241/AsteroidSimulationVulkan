#version 450
layout(early_fragment_tests) in;
layout(location = 0) out vec4 outColor;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in flat int id;
layout(location = 2) in flat int faceFrag;
layout(location = 3) in flat int distanceTest;


layout( std140 ,set = 0,binding = 1) buffer Test
{
    int id;
	int distance;
	int face;
}selctionBuffer;

layout(set = 0,binding = 0) uniform GLobalUbo
{
   mat4 projectionView;
   vec2 cursorPos;
   vec2 windowSize;
   vec3 playerpos;
}ubo;

void main() {
    if(gl_FragCoord.xy == ubo.windowSize+0.5){
        if(distanceTest < selctionBuffer.distance){
            selctionBuffer.id = id;
            selctionBuffer.distance = distanceTest;
            selctionBuffer.face = faceFrag;  
        }
    }
    outColor = vec4(fragColor, 1.0); 
}