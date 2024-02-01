#version 330 core 
#define NR_POINT_LIGHTS 4 

layout(location = 0) out vec4 output;

in vec3 v_FragPos;
in vec4 v_Color;
in vec3 v_Normal;
in vec2 v_UV;
in float v_TextureID;

uniform vec3 u_ViewPos;

void main() { 

	vec3 norm = normalize(v_Normal);
	vec3 viewDir = normalize(u_ViewPos - v_FragPos);

	output = vec4(norm, 1.0);
};

