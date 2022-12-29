#include "BaseLight.h"

BaseLight::BaseLight()
	:
	mColour({1.0f, 1.0f, 1.0f}),
	mAmbientIntensity(0.0f),
	mDiffuseIntensity(0.0f)
{}

BaseLight::BaseLight(glm::vec3 colour, GLfloat ambientIntensity, GLfloat diffuseIntensity)
{
	mColour = colour;
	mAmbientIntensity = ambientIntensity;
	mDiffuseIntensity = diffuseIntensity;
}

BaseLight::~BaseLight()
{}
