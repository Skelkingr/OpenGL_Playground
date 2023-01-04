#include "Camera.h"

Camera::Camera()
	:
	mPosition(0.0f),
	mFront(glm::vec3(0.0f, 0.0f, -1.0f)),
	mUp(0.0f),
	mRight(0.0f),
	mWorldUp(0.0f),
	mYaw(0.0f),
	mPitch(0.0f),
	mMovementSpeed(0.0f),
	mTurnSpeed(0.0f)
{
	Update();
}

Camera::Camera(glm::vec3 position, glm::vec3 worldUp, GLfloat yaw, GLfloat pitch, GLfloat movementSpeed, GLfloat turnSpeed)
	:
	mFront(glm::vec3(0.0f, 0.0f, -1.0f)),
	mUp(0.0f),
	mRight(0.0f)
{
	mPosition = position;
	mWorldUp = worldUp;
	mYaw = yaw;
	mPitch = pitch;
	mMovementSpeed = movementSpeed;
	mTurnSpeed = turnSpeed;

	Update();
}

Camera::~Camera()
{}

void Camera::Update()
{
	mFront.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
	mFront.y = sin(glm::radians(mPitch));
	mFront.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
	
	mFront = glm::normalize(mFront);
	mRight = glm::normalize(glm::cross(mFront, mWorldUp));
	mUp = glm::normalize(glm::cross(mRight, mFront));
}

void Camera::KeyControl(bool* keys, GLfloat deltaTime)
{
	GLfloat velocity = mMovementSpeed * deltaTime;

	if (keys[GLFW_KEY_W])
	{
		mPosition.x += mFront.x * velocity;
		mPosition.z += mFront.z * velocity;
	}

	if (keys[GLFW_KEY_S])
	{
		mPosition.x -= mFront.x * velocity;
		mPosition.z -= mFront.z * velocity;
	}

	if (keys[GLFW_KEY_A])
	{
		mPosition.x -= mRight.x * velocity;
		mPosition.z -= mRight.z * velocity;
	}

	if (keys[GLFW_KEY_D])
	{
		mPosition.x += mRight.x * velocity;
		mPosition.z += mRight.z * velocity;
	}

	Update();
}

void Camera::MouseControl(GLfloat xChange, GLfloat yChange, GLfloat deltaTime)
{
	GLfloat velocity = mTurnSpeed * deltaTime;

	xChange *= velocity;
	yChange *= velocity;

	mYaw += xChange;
	mPitch += yChange;

	if (mPitch > 89.0f)
		mPitch = 89.0f;

	if (mPitch < -89.0f)
		mPitch = -89.0f;

	Update();
}

glm::mat4 Camera::CalculateViewMatrix()
{
	return glm::lookAt(mPosition, mPosition + mFront, mUp);
}
