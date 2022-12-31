#pragma once

#include "BaseLight.h"

class PointLight : public BaseLight
{
public:
	PointLight();
	PointLight(
		glm::vec3 colour, 
		GLfloat ambientIntensity,
		GLfloat diffuseIntensity,
		glm::vec3 position,
		GLfloat exponent,
		GLfloat linear,
		GLfloat constant
	);
	~PointLight();

	void UseLight(
		GLuint ambientColourLocation,
		GLuint ambientIntensityLocation,
		GLuint diffuseIntensityLocation,
		GLuint positionLocation,
		GLuint exponentLocation,
		GLuint linearLocation,
		GLuint constantLocation
	);
	
protected:
	glm::vec3 mPosition;

	GLfloat mExponent;
	GLfloat mLinear;
	GLfloat mConstant;
};

