#ifndef CAMERA_H
#define CAMERA_H

#include "Util.h"

class Camera
{
public:
	Camera();
	Camera(glm::vec3 position, glm::vec3 worldUp, GLfloat yaw, GLfloat pitch, GLfloat movementSpeed, GLfloat turnSpeed);
	~Camera();

	GLvoid Update();

	GLvoid KeyControl(GLboolean* keys, GLfloat deltaTime);
	GLvoid MouseControl(GLfloat xChange, GLfloat yChange, GLfloat deltaTime);

	glm::vec3 GetCameraPosition() const { return mPosition; }
	glm::vec3 GetCameraDirection() const { return glm::normalize(mFront); }

	glm::mat4 CalculateViewMatrix();

private:
	glm::vec3 mPosition;
	glm::vec3 mFront;
	glm::vec3 mUp;
	glm::vec3 mRight;
	glm::vec3 mWorldUp;

	GLfloat mYaw;
	GLfloat mPitch;

	GLfloat mMovementSpeed;
	GLfloat mTurnSpeed;
};

#endif
