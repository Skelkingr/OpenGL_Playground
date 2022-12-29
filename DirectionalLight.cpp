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
