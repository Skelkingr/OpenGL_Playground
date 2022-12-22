#ifndef TRIANGLEAPP_H
#define TRIANGLEAPP_H

#include "App.h"

class CubeApp : public App
{
public:
	CubeApp();
	~CubeApp();

	int Run();

	bool Init();

	void Update(float deltaTime);
	void Render();
private:
	void CreateCube();
	bool AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
	bool CompileShaders();
private:
	GLuint mVAO;
	GLuint mVBO1;
	GLuint mVBO2;
	GLuint mIBO;
	GLuint mShader;

	glm::mat4 mModel;
	glm::mat4 mProjection;
	GLuint mUniformModel;
	GLuint mUniformProjection;

	bool mDirection;
	float mOffset;
	float mMaxOffset;
	float mIncrement;

	float mCurrAngle;

	bool mSizeDirection;
	float mCurrSize;
	float mMaxSize;
	float mMinSize;
};

#endif
