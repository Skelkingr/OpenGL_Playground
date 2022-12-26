#ifndef AMBIENTLIGHT_H
#define AMBIENTLIGHT_H

#include "Light.h"

class DirectionalLight : public Light
{
public:
	DirectionalLight();
	DirectionalLight(GLfloat r, GLfloat g, GLfloat b, GLfloat ambientIntensity);
	~DirectionalLight();
};

#endif

