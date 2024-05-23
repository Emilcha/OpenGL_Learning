#include "FpsCounter.h"

FpsCounter::FpsCounter()
{
	frame();
}

void FpsCounter::frame()
{
	if (frameCount % 100 == 0)
		smoothedFps = (float)(1 / (glfwGetTime() - lastFrameTime));
	lastFrameTime = glfwGetTime();
	frameCount += 1;
}

float FpsCounter::getFps()
{
	return smoothedFps;
}


