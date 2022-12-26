#include "DirectionalLight.h"

DirectionalLight::DirectionalLight()
	:
	Light()
{}

DirectionalLight::DirectionalLight(GLfloat r, GLfloat g, GLfloat b, GLfloat ambientIntensity)
	:
	Light(r, g, b, ambientIntensity)
{}

DirectionalLight::~DirectionalLight()
{}

void DirectionalLight::UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColorLocation)
{
	glUniform3f((GLint)ambientColorLocation, mVecType.x, mVecType.y, mVecType.z);
	glUniform1f((GLint)ambientIntensityLocation, mIntensity);
}
