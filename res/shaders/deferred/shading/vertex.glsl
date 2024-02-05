#version 330 core 

layout(location = 0) in vec3 Position;
layout(location = 1) in vec2 UV;

out vec2 v_UV;

void main() {
   gl_Position = vec4(Position, 1);
   v_UV = UV;
};