#include "PointLight.h"

PointLight::PointLight()
	:
	BaseLight(),
	mPosition({ 0.0f }),
	mExponent(0.0f),
	mLinear(0.0f),
	mConstant(1.0f)
	
{}

PointLight::PointLight(
	glm::vec3 colour, 
	GLfloat ambientIntensity,
	GLfloat diffuseIntensity,
	glm::vec3 position,
	GLfloat exponent,
	GLfloat linear,
	GLfloat constant) :	BaseLight(1024, 1024, colour, ambientIntensity, diffuseIntensity)
{
	mPosition = position;
	mExponent = exponent;
	mLinear = linear;
	mConstant = constant;
}

PointLight::~PointLight()
{}

void PointLight::UseLight(
	GLuint ambientColourLocation,
	GLuint ambientIntensityLocation,
	GLuint diffuseIntensityLocation,
	GLuint positionLocation,
	GLuint exponentLocation,
	GLuint linearLocation,
	GLuint constantLocation)
{
	glUniform3f(ambientColourLocation, mAmbientColour.x, mAmbientColour.y, mAmbientColour.z);
	glUniform1f(ambientIntensityLocation, mAmbientIntensity);
	glUniform1f(diffuseIntensityLocation, mDiffuseIntensity);

	glUniform3f(positionLocation, mPosition.x, mPosition.y, mPosition.z);

	glUniform1f(exponentLocation, mExponent);
	glUniform1f(linearLocation, mLinear);
	glUniform1f(constantLocation, mConstant);
}
