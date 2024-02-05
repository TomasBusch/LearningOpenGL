//Geometry pass (fragment)

#version 330 core

layout (location = 0) out vec4 PositionBuffer;
layout (location = 1) out vec4 NormalBuffer;
layout (location = 2) out vec4 AlbedoRoughBuffer;

struct Material {
    sampler2D albedo;
    sampler2D roughness;
}; 

in vec3 v_FragPos;
in vec4 v_Color;
in vec3 v_Normal;
in vec2 v_UV;
in float v_MaterialID;

uniform vec3 u_ViewPos;
uniform Material material;

void main() { 
	//vec3 viewDir = normalize(u_ViewPos - v_FragPos);

	PositionBuffer = vec4(v_FragPos.xyz, 1);
    NormalBuffer = vec4(normalize(v_Normal), 1);
	AlbedoRoughBuffer.rgb = texture(material.albedo, v_UV).rgb;
	AlbedoRoughBuffer.a = texture(material.roughness, v_UV).r;
};