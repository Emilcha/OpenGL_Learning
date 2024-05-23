#ifndef FPSCOUNTER_H
#define FPSCOUNTER_H

#include <GLFW/glfw3.h>

class FpsCounter
{
public:
	FpsCounter();
	void frame();
	float getFps();
	double lastFrameTime;
	unsigned int frameCount;
	float smoothedFps;
};

#endif