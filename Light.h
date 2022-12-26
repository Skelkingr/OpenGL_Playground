#ifndef LIGHT_H
#define LIGHT_H

#include "Util.h"

class Light
{
public:
	Light();
	Light(GLfloat r, GLfloat g, GLfloat b, GLfloat ambIntensity, GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat difIntensity);
	virtual ~Light();

	void UseLight(
		GLfloat ambientIntensityLocation,
		GLfloat ambientColourLocation,
		GLfloat diffuseIntensityLocation,
		GLfloat directionLocation
	);
																															
private:
	glm::vec3 mColour;
	GLfloat mAmbientIntensity;

	glm::vec3 mDirection;
	GLfloat mDiffuseIntensity;
};

#endif

