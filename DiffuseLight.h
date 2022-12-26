#ifndef DIFFUSELIGHT_H
#define DIFFUSELIGHT_H

#include "Light.h"

class DiffuseLight : public Light
{
	DiffuseLight();
	DiffuseLight(GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat diffuseIntensity);
	~DiffuseLight();
};

