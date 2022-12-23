#ifndef CAMERA_H
#define CAMERA_H

#include "util.h"

class Camera
{
public:
	Camera();
	Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch, GLfloat movementSpeed, GLfloat turnSpeed);
	~Camera();

	void Update();

	void KeyControl(bool* keys);

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
