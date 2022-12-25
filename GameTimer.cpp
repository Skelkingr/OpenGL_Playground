#include "GameTimer.h"

GameTimer::GameTimer()
	:
	mDeltaTime(0.0f),
	mLastTime(0.0f)
{}


GameTimer::~GameTimer()
{}

GLfloat GameTimer::DeltaTime()
{
	GLfloat now = (GLfloat)glfwGetTime();
	mDeltaTime = now - mLastTime;
	mLastTime = now;

	return mDeltaTime;
}
