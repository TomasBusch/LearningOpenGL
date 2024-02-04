#version 330 core
layout(location = 0) in vec4 Position;

out vec3 v_FragPos;

uniform mat4 u_MVP;
uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
    v_FragPos = vec3(Position);
    vec4 pos = u_Projection * u_View * u_Model * Position;
    gl_Position = pos.xyww;
} 