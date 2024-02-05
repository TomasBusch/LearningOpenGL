//Shading pass (fragment)
#version 330 core
#define NR_POINT_LIGHTS 4

layout (location = 0) out vec4 output;

struct DirectionalLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir, vec3 Albedo, float Roughness); 

struct PointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};   
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 Albedo, float Roughness);

struct SpotLight {
    vec3 position;  
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
    float constant;
    float linear;
    float quadratic;
};

vec3 CalcSpotLight(SpotLight spotLight, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 Albedo, float Roughness);

in vec2 v_UV;

uniform vec3 u_ViewPos;
uniform DirectionalLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;

uniform sampler2D PositionBuffer;
uniform sampler2D NormalBuffer;
uniform sampler2D AlbedoRoughBuffer;

void main() { 
    vec3 FragPos = texture(PositionBuffer, v_UV).rgb;
    vec3 Normal = texture(NormalBuffer, v_UV).rgb;
    vec3 Albedo = texture(AlbedoRoughBuffer, v_UV).rgb;
    float Roughness = texture(AlbedoRoughBuffer, v_UV).a;

    vec3 viewDir = normalize(u_ViewPos - FragPos);

    vec3 result = vec3(0.0 ,0.0 ,0.0);
	// phase 1: Directional lighting
    result = CalcDirectionalLight(dirLight, Normal, viewDir, Albedo, Roughness);
    // phase 2: Point lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++){
        result += CalcPointLight(pointLights[i], Normal, FragPos, viewDir, Albedo, Roughness);
    }
    // phase 3: Spot light
    result += CalcSpotLight(spotLight, Normal, FragPos, viewDir, Albedo, Roughness);

	output = vec4(result, 1.0);
};

vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir, vec3 Albedo, float Roughness) {
    vec3 lightDir = normalize(-light.direction);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 255);
    // combine results
    vec3 ambient  = light.ambient * Albedo;
    vec3 diffuse  = light.diffuse * diff * Albedo;
    vec3 specular = light.specular * spec * Roughness;
    return (ambient + diffuse + specular);
};

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 Albedo, float Roughness) {
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(normal, lightDir), 0.0), 255);
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient  = light.ambient * Albedo;
    vec3 diffuse  = light.diffuse * diff * Albedo;
    vec3 specular = light.specular * spec * Roughness;
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
};

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 Albedo, float Roughness) {

    vec3 ambient = light.ambient * Albedo;
    
    // diffuse 
    vec3 lightDir = normalize(light.position - fragPos);
    //lightDir = normalize(fragPos - light.position);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * Albedo;  
    
    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 255);
    vec3 specular = light.specular * spec * Albedo;
    
    // spotlight (soft edges)
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = (light.cutOff - light.outerCutOff);
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    diffuse  *= intensity;
    specular *= intensity;
    
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    ambient  *= attenuation; 
    diffuse   *= attenuation;
    specular *= attenuation;   
        
    vec3 result = ambient + diffuse + specular;
    return result;
};
