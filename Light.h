#ifndef LIGHT_H
#define LIGHT_H

#include "Util.h"

class Light
{
public:
	Light();
	Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat ambientIntensity);
	~Light();

	void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColorLocation);

private:
	glm::vec3 mColour;
	GLfloat mAmbientIntensity;
};

#endif

