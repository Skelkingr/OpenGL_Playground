#pragma once

#include "BaseLight.h"

class PointLight : public BaseLight
{
public:
	PointLight();
	PointLight(glm::vec3 colour, GLfloat ambientIntensity, GLfloat diffuseIntensity, glm::vec3 position, GLfloat exponent, GLfloat linear, GLfloat constant);
	~PointLight();

	void UseLight(
		GLfloat ambientColourLocation,
		GLfloat ambientIntensityLocation,
		GLfloat diffuseIntensityLocation,
		GLfloat positionLocation,
		GLfloat exponentLocation,
		GLfloat linearLocation,
		GLfloat constantLocation
	);
	
private:
	glm::vec3 mPosition;

	GLfloat mExponent;
	GLfloat mLinear;
	GLfloat mConstant;
};

