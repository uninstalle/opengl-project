#ifndef _POST_EFFECT_H
#define _POST_EFFECT_H

enum PostEffect
{
	INVERSION = 1,
	GRAYSCALE = 2,
	SHARPEN = 4,
	BLUR = 8,
	EDGE_DETECTION = 16
};

void initializeScreenFrameBuffer(int screenWidth, int screenHeight);
void enableScreenFrameBuffer();
void disableScreenFrameBuffer();
//Using enum PostEffect
void drawScreenFrameBuffer(unsigned postEffect = 0);


#endif