#ifndef BASELIGHT_H
#define BASELIGHT_H

#include "ShadowMap.h"
#include "Util.h"

class BaseLight
{
public:
	BaseLight();
	BaseLight(GLuint shadowWidth, GLuint shadowHeight, glm::vec3 colour, GLfloat ambientIntensity, GLfloat diffuseIntensity);
	~BaseLight();

	glm::vec3 GetColour() const { return mAmbientColour; }
	GLfloat GetAmbientIntensity() const { return mAmbientIntensity; }
	GLfloat GetDiffuseIntensity() const { return mDiffuseIntensity; }
	ShadowMap* GetShadowMap() const { return mShadowMap; }

protected:
	glm::vec3 mAmbientColour;
	GLfloat mAmbientIntensity;
	GLfloat mDiffuseIntensity;

	glm::mat4 mLightProj;

	ShadowMap* mShadowMap;
};

#endif
