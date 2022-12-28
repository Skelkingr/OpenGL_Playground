#include "DirectionalLight.h"

DirectionalLight::DirectionalLight()
	:
	BaseLight(),
	mDirection({0.0f, -1.0f, 0.0f}),
	mDiffuseIntensity(0.5f)
{}

DirectionalLight::DirectionalLight(glm::vec3 colour, glm::vec3 direction, GLfloat diffuseIntensity)
{
	mColour = colour;
	mDirection = direction;
	mDiffuseIntensity = diffuseIntensity;
}

DirectionalLight::~DirectionalLight()
{}
