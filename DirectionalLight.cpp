#include "DirectionalLight.h"

DirectionalLight::DirectionalLight()
	:
	BaseLight(),
	mDirection({0.0f, 0.0f, 0.0f})
{}

DirectionalLight::DirectionalLight(glm::vec3 colour, GLfloat ambientIntensity, GLfloat diffuseIntensity, glm::vec3 direction)
	:
	BaseLight(colour, ambientIntensity, diffuseIntensity)
{
	mDirection = direction;
}

DirectionalLight::~DirectionalLight()
{}

void DirectionalLight::UseLight(GLfloat ambientColourLocation, GLfloat ambientIntensityLocation, GLfloat diffuseIntensityLocation, GLfloat directionLocation)
{
	glUniform3f(ambientColourLocation, mAmbientColour.x, mAmbientColour.y, mAmbientColour.z);
	glUniform1f(ambientIntensityLocation, mAmbientIntensity);
	glUniform1f(diffuseIntensityLocation, mDiffuseIntensity);
	glUniform3f(directionLocation, mDirection.x, mDirection.y, mDirection.z);
}
