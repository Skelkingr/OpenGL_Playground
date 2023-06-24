#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include "BaseLight.h"

class DirectionalLight : public BaseLight
{
public:
	DirectionalLight();
	DirectionalLight(
		GLfloat shadowWidth,
		GLfloat shadowHeight,
		glm::vec3 colour,
		GLfloat ambientIntensity,
		GLfloat diffuseIntensity,
		GLfloat xDir,
		GLfloat yDir,
		GLfloat zDir
		);
	~DirectionalLight();

	GLvoid UseLight(GLuint ambientColourLocation, GLuint ambientIntensityLocation, GLuint diffuseIntensityLocation, GLuint directionLocation);

	glm::vec3 GetDirection() const { return mDirection; }

	glm::mat4 CalculateLightTransform();

private:
	glm::vec3 mDirection;
};

#endif
