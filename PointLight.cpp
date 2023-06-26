#include "PointLight.h"

PointLight::PointLight()
	:
	BaseLight(),
	mPosition({ 0.0f }),
	mExponent(0.0f),
	mLinear(0.0f),
	mConstant(1.0f),
	mFarPlane(0)	
{}

PointLight::PointLight(
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
	GLfloat constant) :	BaseLight(shadowWidth, shadowHeight, colour, ambientIntensity, diffuseIntensity)
{
	mPosition = position;
	mExponent = exponent;
	mLinear = linear;
	mConstant = constant;

	mFarPlane = far;

	GLfloat aspectRatio = (GLfloat)shadowWidth / (GLfloat)shadowHeight;
	mLightProj = glm::perspective(glm::radians(90.0f), aspectRatio, near, far);

	mShadowMap = new OmniShadowMap();
	mShadowMap->Init(shadowWidth, shadowHeight);
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

std::vector<glm::mat4> PointLight::CalculateLightTransform()
{
	std::vector<glm::mat4> lightMatrices;

	// Positive X, Negative X
	lightMatrices.push_back(mLightProj * glm::lookAt(mPosition, mPosition + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	lightMatrices.push_back(mLightProj * glm::lookAt(mPosition, mPosition + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));

	// Positive Y, Negative Y
	lightMatrices.push_back(mLightProj * glm::lookAt(mPosition, mPosition + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
	lightMatrices.push_back(mLightProj * glm::lookAt(mPosition, mPosition + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));

	// Positive Z, Negative Z
	lightMatrices.push_back(mLightProj * glm::lookAt(mPosition, mPosition + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
	lightMatrices.push_back(mLightProj * glm::lookAt(mPosition, mPosition + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));

	return lightMatrices;
}
