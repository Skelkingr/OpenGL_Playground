#ifndef BASELIGHT_H
#define BASELIGHT_H

#include "Util.h"

class BaseLight
{
public:
	BaseLight();
	BaseLight(glm::vec3 colour, GLfloat ambientIntensity, GLfloat diffuseIntensity);
	~BaseLight();

	glm::vec3 GetColour() const { return mColour; }
	GLfloat GetAmbientIntensity() const { return mAmbientIntensity; }
	GLfloat GetDiffuseIntensity() const { return mDiffuseIntensity; }

protected:
	glm::vec3 mColour;
	GLfloat mAmbientIntensity;
	GLfloat mDiffuseIntensity;
};

#endif
