#version 460
//#extension GL_EXT_shader_explicit_arithmetic_types_int64 : enable

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 color;
layout(location = 2) in int id;
layout(location = 3) in int face;

layout(location = 4) in mat4 offset;
layout(location = 0) out vec3 fragColor;

layout(set = 0,binding = 0) uniform GLobalUbo
{
   mat4 projectionView;
   vec2 cursorPos;
   vec2 windowSize;
   vec3 playerpos;
} globalUbo;


layout(push_constant) uniform Push {
  mat4 transform;
  vec3 color;
} push;

void main() {
    gl_Position = globalUbo.projectionView * push.transform * offset * vec4(pos,1.0);
    fragColor = push.color;
}