#include "BaseLight.h"

BaseLight::BaseLight()
	:
	mColour({1.0f, 1.0f, 1.0f}),
	mAmbientIntensity(0.0f)
{}

BaseLight::BaseLight(glm::vec3 colour, GLfloat ambientIntensity)
{
	mColour = colour;
	mAmbientIntensity = ambientIntensity;
}

BaseLight::~BaseLight()
{}
