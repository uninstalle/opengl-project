#version 450 core
#define MAX_POINTLIGHT_NUM 2


layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec3 VertexNormal;
layout(location = 2) in vec2 VertexTextureCoordinate;

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

out VERTEX_OUT {
	out vec2 TexCoord;
	out vec3 FragPos;
	out vec3 Normal;
};

out LIGHT_OUT {
	DirLight fDirLight;
	PointLight fPointLight[MAX_POINTLIGHT_NUM];
	SpotLight fSpotLight;
};


uniform	DirLight dirLight;
uniform	PointLight pointLight[MAX_POINTLIGHT_NUM];
uniform	SpotLight spotLight;

uniform int numOfDirLights;
uniform int numOfPointLights;
uniform int numOfSpotLights;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(VertexPosition, 1.0);
	TexCoord = VertexTextureCoordinate;
	Normal =vec3(transpose(inverse(view * model)) * vec4(VertexNormal,0.0));


	FragPos = vec3(view * model * vec4(VertexPosition, 1.0));

	if (numOfDirLights>0) {
		fDirLight = dirLight;
		fDirLight.position = vec3(view * vec4(fDirLight.position,0.0));
	}
	//for (int i=0;i<numOfPointLights;++i)
	{
		fPointLight[0] = pointLight[0];
		fPointLight[0].position = vec3( view * vec4(fPointLight[0].position,1.0));
	}
	if (numOfSpotLights>0) {
		fSpotLight = spotLight;
		fSpotLight.position =vec3(view * vec4(fSpotLight.position,1.0));
		fSpotLight.direction =vec3(view * vec4(fSpotLight.direction,0.0));
	}
}