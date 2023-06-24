#include "DirectionalLight.h"

DirectionalLight::DirectionalLight()
	:
	BaseLight()
{
	mDirection = glm::vec3(0.0f, -1.0f, 0.0f);
	mLightProj = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 20.0f);
}

DirectionalLight::DirectionalLight(
	GLfloat shadowWidth,
	GLfloat shadowHeight,
	glm::vec3 colour,
	GLfloat ambientIntensity,
	GLfloat diffuseIntensity,
	GLfloat xDir,
	GLfloat yDir,
	GLfloat zDir
)
	:
	BaseLight(shadowWidth, shadowHeight, colour, ambientIntensity, diffuseIntensity)
{
	mDirection = glm::vec3(xDir, yDir, zDir);
	mLightProj = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 20.0f);
}

DirectionalLight::~DirectionalLight()
{}

GLvoid DirectionalLight::UseLight(
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

glm::mat4 DirectionalLight::CalculateLightTransform()
{
	return mLightProj * glm::lookAt(-mDirection, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}
