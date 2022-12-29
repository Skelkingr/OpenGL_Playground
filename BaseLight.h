#ifndef BASELIGHT_H
#define BASELIGHT_H

#include "Util.h"

class BaseLight
{
public:
	BaseLight();
	BaseLight(glm::vec3 colour, GLfloat ambientIntensity, GLfloat diffuseIntensity);
	~BaseLight();

protected:
	glm::vec3 mAmbientColour;
	GLfloat mAmbientIntensity;
	GLfloat mDiffuseIntensity;
};

#endif
