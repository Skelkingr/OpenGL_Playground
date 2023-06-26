	#pragma once

#include "BaseLight.h"
#include "OmniShadowMap.h"

class PointLight : public BaseLight
{
public:
	PointLight();
	PointLight(
		GLuint shadowWidth,
		GLuint shadowHeight,
		GLfloat near,
		GLfloat far,
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

	std::vector<glm::mat4> CalculateLightTransform();

	glm::vec3 GetPosition() const { return mPosition; }
	GLfloat GetFarPlane() const { return mFarPlane; }

protected:
	glm::vec3 mPosition;

	GLfloat mExponent;
	GLfloat mLinear;
	GLfloat mConstant;

	GLfloat mFarPlane;
};

