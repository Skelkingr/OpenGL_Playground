#include "BaseLight.h"

BaseLight::BaseLight()
	:
	mAmbientColour({1.0f, 1.0f, 1.0f}),
	mAmbientIntensity(0.0f),
	mDiffuseIntensity(0.0f),
	mLightProj({0.0f}),
	mShadowMap(nullptr)
{}

BaseLight::BaseLight(GLuint shadowWidth, GLuint shadowHeight, glm::vec3 ambientColour, GLfloat ambientIntensity, GLfloat diffuseIntensity)
	:
	mLightProj({ 0.0f })
{
	mShadowMap = new ShadowMap();
	mShadowMap->Init(shadowWidth, shadowHeight);

	mAmbientColour = ambientColour;
	mAmbientIntensity = ambientIntensity;
	mDiffuseIntensity = diffuseIntensity;
}

BaseLight::~BaseLight()
{}
