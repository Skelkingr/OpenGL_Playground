#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include "PointLight.h"

class SpotLight : public PointLight
{
public:
	SpotLight();
	SpotLight(
		GLuint shadowWidth,
		GLuint shadowHeight,
		GLfloat near,
		GLfloat far,
		glm::vec3 colour,
		GLfloat ambientIntensity,
		GLfloat diffuseIntensity,
		glm::vec3 position,
		glm::vec3 direction,
		GLfloat exponent,
		GLfloat linear,
		GLfloat constant,
		GLfloat edge
	);
	~SpotLight();

	GLvoid UseLight(
		GLuint ambientColourLocation,
		GLuint ambientIntensityLocation,
		GLuint diffuseIntensityLocation,
		GLuint positionLocation,
		GLuint directionLocation,
		GLuint exponentLocation,
		GLuint linearLocation,
		GLuint constantLocation,
		GLuint edgeLocation
	);

	GLvoid SetFlash(glm::vec3 pos, glm::vec3 dir);
	GLvoid Toggle() { mIsOn = !mIsOn; }

private:
	glm::vec3 mDirection;

	GLfloat mEdge;
	GLfloat mProcEdge;

	GLboolean mIsOn;
};

#endif

