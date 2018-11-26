#version 330 core
#define PROCESS_INVERSION 1
#define PROCESS_GRAYSCALE 2
#define PROCESS_SHARPEN 4
#define PROCESS_BLUR 8
#define PROCESS_EDGE_DETECTION 16

out vec4 FragColor;
in vec2 TexCoord;
uniform sampler2D ScreenTexture;
uniform int ProcessMode;


void main()
{
	if (ProcessMode >= PROCESS_SHARPEN)
		{
			const float offset = 1.0 / 300.0;  
			vec2 offsets[9] = vec2[](
			vec2(-offset,  offset),
			vec2( 0.0f,    offset),
			vec2( offset,  offset),
			vec2(-offset,  0.0f),
			vec2( 0.0f,    0.0f), 
			vec2( offset,  0.0f), 
			vec2(-offset, -offset),
			vec2( 0.0f,   -offset), 
			vec2( offset, -offset));

			float kernel[9];
			if ((ProcessMode & PROCESS_SHARPEN) != 0)
			kernel = float[](
			 -1, -1, -1,
			 -1,  9, -1,
			 -1, -1, -1);
			else if ((ProcessMode & PROCESS_BLUR) != 0)
			kernel = float[](
			 1.0 / 16, 2.0 / 16, 1.0 / 16,
			 2.0 / 16, 4.0 / 16, 2.0 / 16,
			 1.0 / 16, 2.0 / 16, 1.0 / 16  );
			else if ((ProcessMode & PROCESS_EDGE_DETECTION) != 0)
			kernel = float[](
			 1, 1, 1,
			 1,  -8, 1,
			 1, 1, 1);

			vec3 sampleTex[9];
			for(int i = 0; i < 9; i++)
			{
				 sampleTex[i] = vec3(texture(ScreenTexture, TexCoord.st + offsets[i]));
			}
			vec3 col = vec3(0.0);
			for(int i = 0; i < 9; i++)
				col += sampleTex[i] * kernel[i];

			FragColor = vec4(col, 1.0);
		}
	else
		FragColor = texture(ScreenTexture,TexCoord);
	if ((ProcessMode & PROCESS_INVERSION) != 0)
		FragColor = vec4(vec3(1.0 - FragColor), 1.0);
	if ((ProcessMode & PROCESS_GRAYSCALE) != 0)
		{
			float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
			FragColor = vec4(average, average, average, 1.0);
		}

	// Gamma correction
    // Dont use it now, light computing isnt updated yet.
	//float gamma = 2.2;
    //FragColor.rgb = pow(FragColor.rgb, vec3(1.0/gamma));
	
}