#ifndef GAMETIMER_H
#define GAMETIMER_H

#include "util.h"

class GameTimer
{
public:
	GameTimer();
	~GameTimer();

	GLfloat DeltaTime();

private:
	GLfloat mDeltaTime;
	GLfloat mLastTime;
};

#endif

