#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include "BaseLight.h"

class DirectionalLight : public BaseLight
{
public:
	DirectionalLight();
	DirectionalLight(glm::vec3 colour, GLfloat ambientIntensity, GLfloat diffuseIntensity, glm::vec3 direction);
	~DirectionalLight();

	void UseLight(GLuint ambientColourLocation, GLuint ambientIntensityLocation, GLuint diffuseIntensityLocation, GLuint directionLocation);

	glm::vec3 GetDirection() const { return mDirection; }
private:
	glm::vec3 mDirection;
};

#endif
