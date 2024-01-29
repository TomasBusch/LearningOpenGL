#version 330 core 

layout(location = 0) in vec4 Position;
layout(location = 1) in vec4 Color;
layout(location = 2) in vec3 Normal;
layout(location = 3) in vec2 UV;
layout(location = 4) in float TextureID;

out vec4 o_Color;
out vec3 o_Normal;
out vec2 o_UV;
out float o_TextureID;

uniform mat4 u_MVP;

void main() {
   //gl_Position = u_MVP * Position;
   gl_Position = Position;
   o_Color = Color;
   o_Normal = Normal;
   o_UV = UV;
   o_TextureID = TextureID;
};