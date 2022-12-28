#include "DirectionalLight.h"

DirectionalLight::DirectionalLight()
	:
	BaseLight()
{}

DirectionalLight::DirectionalLight(glm::vec3 colour, glm::vec3 direction, GLfloat diffuseIntensity)
{
	mColour = colour;
	mDirection = direction;
	mDiffuseIntensity = diffuseIntensity;
}



DirectionalLight::~DirectionalLight()
{}
