#include "Light.h"

Light::Light()
	:
	mColour({1.0f}),
	mAmbientIntensity(0.0f)
{}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat ambientIntensity)
{
	mColour = glm::vec3(red, green, blue);
	mAmbientIntensity = ambientIntensity;
}


Light::~Light()
{}

void Light::UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColorLocation)
{
	glUniform1f(ambientIntensityLocation, mAmbientIntensity);
	glUniform3f(ambientColorLocation, mColour.x, mColour.y, mColour.z);
}
