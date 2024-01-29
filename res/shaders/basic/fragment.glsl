#version 330 core 

layout(location = 0) out vec4 output;

in vec4 v_Color;
in vec3 v_Normal;
in vec2 v_UV;
in float v_TextureID;

//uniform vec4 u_Color;
uniform sampler2D u_Textures[2];

void main() { 
	int TextureID = int(v_TextureID);
	vec4 texColor = texture(u_Textures[TextureID], v_UV);
	output = texColor;

	//output = o_Color;
};