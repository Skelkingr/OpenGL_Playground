#include "Light.h"

Light::Light()
	:
	mVecType({1.0f}),
	mIntensity(1.0f)
{}

Light::Light(GLfloat x, GLfloat y, GLfloat z, GLfloat ambientIntensity)
{
	mVecType = glm::vec3(x, y, z);
	mIntensity = ambientIntensity;
}


Light::~Light()
{}

void Light::UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColorLocation)
{
	glUniform1f(ambientIntensityLocation, mIntensity);
	glUniform3f(ambientColorLocation, mVecType.x, mVecType.y, mVecType.z);
}
