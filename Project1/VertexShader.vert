#version 330 core
layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec3 VertexNormal;
layout(location = 2) in vec2 VertexTextureCoordinate;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;
out mat4 ViewMat;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(VertexPosition, 1.0);
	TexCoord = vec2(VertexTextureCoordinate.x, VertexTextureCoordinate.y);
	FragPos = vec3(view * model * vec4(VertexPosition, 1.0));
	Normal = vec3(transpose(inverse(view * model)) * vec4(VertexNormal,0.0));
	ViewMat = view;
}