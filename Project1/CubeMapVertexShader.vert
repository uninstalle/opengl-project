#version 330 core

layout (location = 0) in vec3 VertexPosition;

out vec3 TexCoord;

uniform mat4 projection;
uniform mat4 view;

void main()
{
	TexCoord = VertexPosition;
	vec4 pos = projection * mat4(mat3(view)) * vec4(VertexPosition,1.0);
	gl_Position = pos.xyww;
}