#version 450 core
#define MAX_POINTLIGHT_NUM 2

struct Material {
	sampler2D texture_diffuse1;
	sampler2D texture_diffuse2;
	sampler2D texture_specular1;
	sampler2D texture_specular2;
	sampler2D texture_normal1;
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

in VERTEX_OUT {
	in vec2 TexCoord;
	in vec3 FragPos;
} vs_result;

uniform Material material;

in LIGHT_OUT {
	DirLight fDirLight;
	PointLight fPointLight[MAX_POINTLIGHT_NUM];
	SpotLight fSpotLight;
};

uniform int numOfDirLights;
uniform int numOfPointLights;
uniform int numOfSpotLights;

vec4 CalDirLight(DirLight light, vec3 normal, vec3 fragmentPosition);
vec4 CalPointLight(PointLight light, vec3 normal, vec3 fragmentPosition);
vec4 CalSpotLight(SpotLight light, vec3 normal, vec3 fragmentPosition);

void main()
{
// normal is in tangent space
	vec3 normal = texture(material.texture_normal1,vs_result.TexCoord).rgb;
	normal = normalize(normal*2.0-1.0);
	vec4 result = vec4(0.0);
	if (numOfDirLights>0)
		result = CalDirLight(fDirLight, normal, vs_result.FragPos);
	for (int i = 0; i < numOfPointLights; i++)
		result += CalPointLight(fPointLight[i], normal, vs_result.FragPos);
	if (numOfSpotLights>0)
		result += CalSpotLight(fSpotLight, normal, vs_result.FragPos);

	FragColor = vec4(normal,1.0);
	//FragColor = result;
}


vec4 CalDirLight(DirLight light, vec3 normal, vec3 fragmentPosition)
{
	vec3 lightDirection = normalize(light.position);
	float diff = max(dot(normal, -lightDirection), 0.0);
	vec3 viewDirection = normalize(fragmentPosition);
	vec3 halfwayDirection = -normalize(lightDirection + viewDirection);
	float spec = pow(max(dot(normal, halfwayDirection), 0.0), material.shininess);

	vec4 diffuseTex = texture(material.texture_diffuse1, vs_result.TexCoord);
	vec4 specularTex = texture(material.texture_specular1,vs_result.TexCoord);
	
	vec3 ambient = light.ambient*vec3(diffuseTex);
	vec3 diffuse = light.diffuse*diff*vec3(diffuseTex);
	vec3 specular = light.specular*spec*vec3(specularTex);

	return vec4(ambient + diffuse + specular,diffuseTex.a);
}

vec4 CalPointLight(PointLight light, vec3 normal, vec3 fragmentPosition)
{
// Calculate light in tangent space
// viewDirection: From viewer's position to fragment position
	vec3 viewDirection = normalize(fragmentPosition);
// lightDirection: From light source to fragment position
	vec3 lightDirection = normalize(fragmentPosition - light.position);
	float diff = max(dot(normal, -lightDirection), 0.0);
	vec3 halfwayDirection = -normalize(lightDirection + viewDirection);
	float spec = pow(max(dot(normal, halfwayDirection), 0.0), material.shininess);

	float distance = length(fragmentPosition - light.position);
	float attenuation = 1.0 / (light.constant + light.linear*distance + light.quadratic*(distance*distance));
// Keep the A of texture's RGBA unchanged during light computing
	vec4 diffuseTex = texture(material.texture_diffuse1, vs_result.TexCoord);
	vec4 specularTex = texture(material.texture_specular1,vs_result.TexCoord);

	vec3 ambient = light.ambient*vec3(diffuseTex);
	vec3 diffuse = light.diffuse*diff*vec3(diffuseTex);
	vec3 specular = light.specular*spec*vec3(specularTex);

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return vec4(ambient + diffuse + specular,diffuseTex.a);
}

vec4 CalSpotLight(SpotLight light, vec3 normal, vec3 fragmentPosition)
{
	vec3 viewDirection = normalize(fragmentPosition);
	vec3 lightDirection = normalize(fragmentPosition - light.position);
	float theta = dot(lightDirection, normalize(light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

	float diff = max(dot(normal, -lightDirection), 0.0);
	vec3 halfwayDirection = -normalize(lightDirection + viewDirection);
	float spec = pow(max(dot(normal, halfwayDirection), 0.0), material.shininess);
	float distance = length(fragmentPosition - light.position);
	float attenuation = 1.0 / (light.constant + light.linear*distance + light.quadratic*(distance*distance));
	
	vec4 diffuseTex = texture(material.texture_diffuse1, vs_result.TexCoord);
	vec4 specularTex = texture(material.texture_specular1,vs_result.TexCoord);

	vec3 ambient = light.ambient*vec3(diffuseTex);
	vec3 diffuse = light.diffuse*diff*vec3(diffuseTex);
	vec3 specular = light.specular*spec*vec3(specularTex);

	ambient *= attenuation;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;

	return vec4(ambient + diffuse + specular,diffuseTex.a);
}