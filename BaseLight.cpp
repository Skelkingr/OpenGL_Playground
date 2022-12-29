#include "BaseLight.h"

BaseLight::BaseLight()
	:
	mAmbientColour({1.0f, 1.0f, 1.0f}),
	mAmbientIntensity(0.0f),
	mDiffuseIntensity(0.0f)
{}

BaseLight::BaseLight(glm::vec3 ambientColour, GLfloat ambientIntensity, GLfloat diffuseIntensity)
{
	mAmbientColour = ambientColour;
	mAmbientIntensity = ambientIntensity;
	mDiffuseIntensity = diffuseIntensity;
}

BaseLight::~BaseLight()
{}
