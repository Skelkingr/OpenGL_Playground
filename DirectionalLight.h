#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include "BaseLight.h"

class DirectionalLight : public BaseLight
{
public:
	DirectionalLight();
	DirectionalLight(glm::vec3 colour, glm::vec3 direction, GLfloat diffuseIntensity);
	~DirectionalLight();

	glm::vec3 GetDirection() const { return mDirection; }
	GLfloat GetDiffuseIntensity() const { return mDiffuseIntensity; }

private:
	glm::vec3 mDirection;
	GLfloat mDiffuseIntensity;
};

#endif
