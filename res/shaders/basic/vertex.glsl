#version 330 core 

layout(location = 0) in vec4 Position;
layout(location = 1) in vec4 Color;
layout(location = 2) in vec3 Normal;
layout(location = 3) in vec2 UV;
layout(location = 4) in float TextureID;

out vec4 v_Color;
out vec3 v_Normal;
out vec2 v_UV;
out float v_TextureID;

uniform mat4 u_MVP;
uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main() {
   gl_Position = u_Projection * u_View * u_Model * Position;
   //gl_Position = Position;
   v_Color = Color;
   v_Normal = Normal;
   v_UV = UV;
   v_TextureID = TextureID;
};