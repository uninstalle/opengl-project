#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;

out vec2 texCoord;
out vec3 Normal;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view*model*vec4(aPos, 1.0);
	texCoord = vec2(aTexCoord.x, aTexCoord.y);
	fragPos = vec3(model*vec4(aPos, 1.0));
	Normal = mat3(transpose(inverse(model)))*aPos;
}