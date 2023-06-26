#include "SpotLight.h"

SpotLight::SpotLight()
	:
	PointLight(),
	mEdge(0.0f)
{
	mDirection = glm::vec3(0.0f, -1.0f, 0.0f);
	mProcEdge = cosf(glm::radians(mEdge));
}

SpotLight::SpotLight(
	GLuint shadowWidth,
	GLuint shadowHeight,
	GLfloat near,
	GLfloat far,
	glm::vec3 colour, 
	GLfloat ambientIntensity, 
	GLfloat diffuseIntensity, 
	glm::vec3 position,
	glm::vec3 direction,
	GLfloat exponent, 
	GLfloat linear, 
	GLfloat constant,
	GLfloat edge) : PointLight(shadowWidth, shadowHeight, near, far, colour, ambientIntensity, diffuseIntensity, position, exponent, linear, constant)
{
	mDirection = glm::normalize(direction);
	mEdge = edge;
	mProcEdge = cosf(glm::radians(mEdge));
}

SpotLight::~SpotLight()
{}

void SpotLight::UseLight(
	GLuint ambientColourLocation,
	GLuint ambientIntensityLocation,
	GLuint diffuseIntensityLocation,
	GLuint positionLocation,
	GLuint directionLocation,
	GLuint exponentLocation,
	GLuint linearLocation,
	GLuint constantLocation,
	GLuint edgeLocation)
{
	glUniform3f(ambientColourLocation, mAmbientColour.x, mAmbientColour.y, mAmbientColour.z);
	glUniform1f(ambientIntensityLocation, mAmbientIntensity);
	glUniform1f(diffuseIntensityLocation, mDiffuseIntensity);

	glUniform3f(positionLocation, mPosition.x, mPosition.y, mPosition.z);
	glUniform3f(directionLocation, mDirection.x, mDirection.y, mDirection.z);

	glUniform1f(exponentLocation, mExponent);
	glUniform1f(linearLocation, mLinear);
	glUniform1f(constantLocation, mConstant);
	
	glUniform1f(edgeLocation, mProcEdge);
}

void SpotLight::SetFlash(glm::vec3 pos, glm::vec3 dir)
{
	mPosition = pos;
	mDirection = dir;
}
