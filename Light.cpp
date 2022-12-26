#include "Light.h"

Light::Light()
{
	mColour = glm::vec3(1.0f, 1.0f, 1.0f);
	mAmbientIntensity = 1.0f;

	mDirection = glm::vec3(0.0f, -1.0f, 0.0f);
	mDiffuseIntensity = 0.0f;
}

Light::Light(GLfloat r, GLfloat g, GLfloat b, GLfloat ambIntensity, GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat difIntensity)
{
	mColour = glm::vec3(r, g, b);
	mAmbientIntensity = ambIntensity;

	mDirection = glm::vec3(xDir, yDir, zDir);
	mDiffuseIntensity = difIntensity;
}

Light::~Light()
{}

void Light::UseLight(
	GLfloat ambientIntensityLocation,
	GLfloat ambientColourLocation,
	GLfloat diffuseIntensityLocation,
	GLfloat directionLocation
)
{
	glUniform3f(ambientColourLocation, mColour.x, mColour.y, mColour.z);
	glUniform1f(ambientIntensityLocation, mAmbientIntensity);
	glUniform3f(directionLocation, mDirection.x, mDirection.y, mDirection.z);
	glUniform1f(diffuseIntensityLocation, mDiffuseIntensity);
}
