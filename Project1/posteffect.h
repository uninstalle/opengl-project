#ifndef _POST_EFFECT_H
#define _POST_EFFECT_H

void initializeScreenFrameBuffer(int screenWidth, int screenHeight);
void enableScreenFrameBuffer();
void disableScreenFrameBuffer();
void drawScreenFrameBuffer(unsigned postEffect = 0);


#endif