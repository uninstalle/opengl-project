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

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
in mat4 ViewMat;

uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLight[MAX_POINTLIGHT_NUM];
uniform SpotLight spotLight;

uniform int isSpotLightOn;

uniform int PointLightNum;

vec4 CalDirLight(DirLight light, vec3 normal, vec3 fragmentPosition);
vec4 CalPointLight(PointLight light, vec3 normal, vec3 fragmentPosition);
vec4 CalSpotLight(SpotLight light, vec3 normal, vec3 fragmentPosition);

void main()
{
	vec3 normal = normalize(Normal);
// viewDirection: from view point(0,0,0) to fragment position,
// which is also the fragment position in view coordinate system.

	vec4 result = CalDirLight(dirLight, normal, FragPos);
	for (int i = 0; i < PointLightNum; i++)
		result += CalPointLight(pointLight[i], normal, FragPos);
	if (isSpotLightOn == 1)
		result += CalSpotLight(spotLight, normal, FragPos);

	FragColor = result;
}


vec4 CalDirLight(DirLight light, vec3 normal, vec3 fragmentPosition)
{
	light.position = vec3(ViewMat * vec4(light.position,0.0));
	vec3 lightDirection = normalize(light.position);
	float diff = max(dot(normal, -lightDirection), 0.0);
	vec3 viewDirection = normalize(fragmentPosition);
	vec3 halfwayDirection = normalize(lightDirection + viewDirection);
	float spec = pow(max(dot(normal, halfwayDirection), 0.0), material.shininess);

	vec4 diffuseTex = texture(material.texture_diffuse1, TexCoord);
	vec4 specularTex = texture(material.texture_specular1,TexCoord);

	vec4 ambient =  vec4(light.ambient,1.0) * diffuseTex;
	vec4 diffuse = vec4(light.diffuse,1.0) * diff * diffuseTex;
	vec4 specular = vec4(light.specular,1.0) * spec * specularTex;

	return ambient + diffuse + specular;
}

vec4 CalPointLight(PointLight light, vec3 normal, vec3 fragmentPosition)
{
// All vectors from uniform that are in world coordinate system need to be conveyed
// to view coordinate system.
	light.position = vec3( ViewMat * vec4(light.position,1.0));
// viewDirection: From (0,0,0) to fragment position
	vec3 viewDirection = normalize(fragmentPosition);
// lightDirection: From light source to fragment position
	vec3 lightDirection = normalize(fragmentPosition - light.position);
	float diff = max(dot(normal, -lightDirection), 0.0);
	vec3 halfwayDirection = normalize(lightDirection + viewDirection);
	float spec = pow(max(dot(normal, halfwayDirection), 0.0), material.shininess);

	float distance = length(fragmentPosition - light.position);
	float attenuation = 1.0 / (light.constant + light.linear*distance + light.quadratic*(distance*distance));
	
	vec4 diffuseTex = texture(material.texture_diffuse1, TexCoord);
	vec4 specularTex = texture(material.texture_specular1,TexCoord);

	vec4 ambient = vec4(light.ambient,1.0)*diffuseTex;
	vec4 diffuse = vec4(light.diffuse,1.0)*diff*diffuseTex;
	vec4 specular = vec4(light.specular,1.0)*spec*specularTex;

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return ambient + diffuse + specular;
}

vec4 CalSpotLight(SpotLight light, vec3 normal, vec3 fragmentPosition)
{
	light.position = vec3( ViewMat * vec4(light.position,1.0));
	light.direction = vec3( ViewMat * vec4(light.direction,0.0));
	vec3 viewDirection = normalize(fragmentPosition);
	vec3 lightDirection = normalize(fragmentPosition - light.position);
	float theta = dot(lightDirection, normalize(light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

	float diff = max(dot(normal, -lightDirection), 0.0);
	vec3 halfwayDirection = normalize(lightDirection + viewDirection);
	float spec = pow(max(dot(normal, halfwayDirection), 0.0), material.shininess);
	float distance = length(fragmentPosition - light.position);
	float attenuation = 1.0 / (light.constant + light.linear*distance + light.quadratic*(distance*distance));
	
	vec4 diffuseTex = texture(material.texture_diffuse1, TexCoord);
	vec4 specularTex = texture(material.texture_specular1,TexCoord);

	vec4 ambient = vec4(light.ambient,1.0)*diffuseTex;
	vec4 diffuse = vec4(light.diffuse,1.0)*diff*diffuseTex;
	vec4 specular = vec4(light.specular,1.0)*spec*specularTex;

	ambient *= attenuation;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;

	return ambient + diffuse + specular;
}