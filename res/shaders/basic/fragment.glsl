#version 330 core 

layout(location = 0) out vec4 output;

in vec3 v_FragPos;
in vec4 v_Color;
in vec3 v_Normal;
in vec2 v_UV;
in float v_TextureID;

uniform sampler2D u_Textures[2];
uniform vec4 u_AmbientLight;
uniform vec3 u_PointLightPos;
uniform vec4 u_PointLight;
uniform vec3 u_ViewPos;

struct Material {
    sampler2D diffuse;
    sampler2D specular;    
    float shininess;
}; 
uniform Material material;

struct DirectionalLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirectionalLight dirLight;
vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir); 

struct PointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
#define NR_POINT_LIGHTS 4  
uniform PointLight pointLights[NR_POINT_LIGHTS];

void main() { 
	//Add .5 to avoid interpolation and precision error changing texture ID
	int TextureID = int(v_TextureID + 0.5);
	vec4 albedo = texture(u_Textures[TextureID], v_UV);

	vec3 norm = normalize(v_Normal);
	vec3 lightDir = normalize(u_PointLightPos - v_FragPos);
	float diff = max(dot(v_Normal, lightDir), 0.0);
	vec4 diffuse = diff * u_PointLight;

	int SpecularTextureID = int(v_TextureID + 1.5);
	float specularStrength = 0.5;
	vec3 viewDir = normalize(u_ViewPos - v_FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	int spec_exp = 32;
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), spec_exp);
	vec4 specular = specularStrength * spec * u_PointLight * texture(u_Textures[SpecularTextureID], v_UV);

	output =  (u_AmbientLight + diffuse + specular) * albedo;
};

vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, v_UV));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, v_UV));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, v_UV));
    return (ambient + diffuse + specular);
};

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, v_UV));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, v_UV));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, v_UV));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
} 