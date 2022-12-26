#include "DiffuseLight.h"

DiffuseLight::DiffuseLight()
{
	mVecType = glm::vec3(0.0f, -1.0f, 0.0f);
	mIntensity = 0.0f;
}

DiffuseLight::DiffuseLight(GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat diffuseIntensity)
{
	mVecType = glm::vec3(xDir, yDir, zDir);
	mIntensity = diffuseIntensity;
}

DiffuseLight::~DiffuseLight()
{}
