#ifndef TRIANGLEAPP_H
#define TRIANGLEAPP_H

#include "App.h"

class TriangleApp : public App
{
public:
	TriangleApp();
	~TriangleApp();

	int Run();

	bool Init();

	void Update(float deltaTime);
	void Render();
private:
	void CreateTriangle();
	bool AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
	bool CompileShaders();
private:
	GLuint mVAO;
	GLuint mVBO1;
	GLuint mVBO2;
	GLuint mShader;

	glm::mat4 mModel;
	GLuint mUniformModel;

	bool mDirection;
	float mCurrAngle;
	float mTriOffset;
	float mTriMaxOffset;
	float mTriIncrement;
};

#endif
