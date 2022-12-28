#ifndef BASELIGHT_H
#define BASELIGHT_H

#include "Util.h"

class BaseLight
{
public:
	BaseLight();
	BaseLight(glm::vec3 colour, GLfloat ambientIntensity);
	~BaseLight();

private:
	glm::vec3 mColour;
	GLfloat mAmbientIntensity;
};

#endif
