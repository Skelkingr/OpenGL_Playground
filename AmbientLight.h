#ifndef AMBIENTLIGHT_H
#define AMBIENTLIGHT_H

#include "Light.h"

class AmbientLight : public Light
{
public:
	AmbientLight();
	AmbientLight(GLfloat r, GLfloat g, GLfloat b, GLfloat ambientIntensity);
	~AmbientLight();
};

#endif

