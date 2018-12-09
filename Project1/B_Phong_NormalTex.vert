#version 450 core
#define MAX_POINTLIGHT_NUM 2


layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec3 VertexNormal;
layout(location = 2) in vec2 VertexTextureCoordinate;
layout(location = 3) in vec3 VertexTangent;
layout(location = 4) in vec3 VertexBiTangent;

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
	out vec3 ViewPos;
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

uniform vec3 ViewPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(VertexPosition, 1.0);
	TexCoord = VertexTextureCoordinate;

	vec3 T = normalize(vec3(model*vec4(VertexTangent,0.0)));
	vec3 N = normalize(vec3(model*vec4(VertexNormal,0.0)));
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(T, N);
	mat3 TBN = transpose(mat3(T,B,N));

	FragPos = TBN * vec3(model * vec4(VertexPosition, 1.0));
	ViewPos = TBN * ViewPosition;

	if (numOfDirLights>0) {
		fDirLight = dirLight;
		fDirLight.position = TBN * fDirLight.position;
	}
	//for (int i=0;i<numOfPointLights;++i)
	{
		fPointLight[0] = pointLight[0];
		fPointLight[0].position = TBN * fPointLight[0].position;
	}
	if (numOfSpotLights>0) {
		fSpotLight = spotLight;
		fSpotLight.position = TBN * fSpotLight.position;
		fSpotLight.direction = TBN * fSpotLight.direction;
	}
}