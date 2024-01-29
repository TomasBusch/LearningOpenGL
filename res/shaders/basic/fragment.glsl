#version 330 core 

layout(location = 0) out vec4 output;

in vec4 o_Color;
in vec3 o_Normal;
in vec2 o_UV;
in float o_TextureID;

//uniform vec4 u_Color;
uniform sampler2D u_Textures[2];

void main() { 
	int TextureID = int(o_TextureID);
	vec4 texColor = texture(u_Textures[TextureID], o_UV);
	output = texColor;

	//output = o_Color;
};