#version 330 core
#define MAX_POINTLIGHT_NUM 4

struct Material {
	sampler2D texture_diffuse1;
	sampler2D texture_diffuse2;
	sampler2D texture_diffuse3;
	sampler2D texture_specular1;
	sampler2D texture_specular2;
	sampler2D texture_specular3;
	float shininess;
};
struct DirLight {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
struct PointLight {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float constant;
	float linear;
	float quadratic;
};
struct SpotLight {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 direction;
	float constant;
	float linear;
	float quadratic;
	float cutOff;
	float outerCutOff;
};


out vec4 FragColor;
in vec2 texCoord;

in vec3 Normal;
in vec3 fragPos;

uniform vec3 viewPos;

uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLight[MAX_POINTLIGHT_NUM];
uniform SpotLight spotLight;

uniform int isSpotLightOn;

uniform int PointLightNum;

vec3 CalDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
	vec3 normal = normalize(Normal);
	vec3 viewDir = normalize(viewPos - fragPos);

	vec3 result = CalDirLight(dirLight, normal, viewDir);
	for (int i = 0; i < PointLightNum; i++)
		result += CalPointLight(pointLight[i], normal, fragPos, viewDir);
	if (isSpotLightOn == 1)
		result += CalSpotLight(spotLight, normal, fragPos, viewDir);

	FragColor = vec4(result, 1.0);
}


vec3 CalDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.position);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, texCoord));
	vec3 diffuse = light.diffuse * diff*vec3(texture(material.texture_diffuse1, texCoord));
	vec3 specular = light.specular *spec*vec3(texture(material.texture_specular1, texCoord));
	return ambient + diffuse + specular;
}

vec3 CalPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear*distance + light.quadratic*(distance*distance));
	vec3 ambient = light.ambient*vec3(texture(material.texture_diffuse1, texCoord));
	vec3 diffuse = light.diffuse*diff*vec3(texture(material.texture_diffuse1, texCoord));
	vec3 specular = light.specular*spec*vec3(texture(material.texture_specular1, texCoord));
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	return ambient + diffuse + specular;
}

vec3 CalSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

	float diff = max(dot(normal, lightDir), 0.0);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear*distance + light.quadratic*(distance*distance));
	vec3 ambient = light.ambient*vec3(texture(material.texture_diffuse1, texCoord));
	vec3 diffuse = light.diffuse*diff*vec3(texture(material.texture_diffuse1, texCoord));
	vec3 specular = light.specular*spec*vec3(texture(material.texture_specular1, texCoord));
	ambient *= attenuation;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;
	return ambient + diffuse + specular;
}