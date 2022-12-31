#include "DirectionalLight.h"

DirectionalLight::DirectionalLight()
	:
	BaseLight(),
	mDirection({ 0.0f })
{}

DirectionalLight::DirectionalLight(glm::vec3 colour, GLfloat ambientIntensity, GLfloat diffuseIntensity, glm::vec3 direction)
	:
	BaseLight(colour, ambientIntensity, diffuseIntensity)
{
	mDirection = direction;
}

DirectionalLight::~DirectionalLight()
{}

void DirectionalLight::UseLight(
	GLuint ambientColourLocation,
	GLuint ambientIntensityLocation,
	GLuint diffuseIntensityLocation,
	GLuint directionLocation)
{
	glUniform3f(
		ambientColourLocation,
		mAmbientColour.x,
		mAmbientColour.y,
		mAmbientColour.z
	);
	glUniform1f(ambientIntensityLocation, mAmbientIntensity);
	glUniform3f(
		directionLocation,
		mDirection.x,
		mDirection.y,
		mDirection.z
	);
	glUniform1f(diffuseIntensityLocation, mDiffuseIntensity);
}
