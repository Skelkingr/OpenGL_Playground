#ifndef LIGHT_H
#define LIGHT_H

#include "Util.h"

class Light
{
public:
	Light();
	Light(GLfloat x, GLfloat y, GLfloat z, GLfloat intensity);
	virtual ~Light();

	void UseLight(GLfloat intensityLocation, GLfloat typeLocation);

protected:
	glm::vec3 mVecType;
	GLfloat mIntensity;
};

#endif

