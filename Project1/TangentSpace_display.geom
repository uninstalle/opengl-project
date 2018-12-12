#version 450 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in GEO_OUT {
	mat3 oTBN;
} ge_in[];

out vec3 fColor;

void main()
{
//T
    gl_Position = gl_in[0].gl_Position;
	fColor = vec3(1,0,0);
	EmitVertex();
	gl_Position = gl_in[0].gl_Position + vec4(ge_in[0].oTBN[0] ,0.0)*0.05;
	fColor = vec3(1,0,0);
	EmitVertex();
	EndPrimitive();
//B
	gl_Position = gl_in[0].gl_Position;
	fColor = vec3(0,1,0);
	EmitVertex();
	gl_Position = gl_in[0].gl_Position + vec4(ge_in[0].oTBN[1],0.0)*0.05;
	fColor = vec3(0,1,0);
	EmitVertex();
	EndPrimitive();
//N
	gl_Position = gl_in[0].gl_Position;
	fColor = vec3(0,0,1);
	EmitVertex();
	gl_Position = gl_in[0].gl_Position + vec4(ge_in[0].oTBN[2],0.0)*0.05;
	fColor = vec3(0,0,1);
	EmitVertex();
	EndPrimitive();
}