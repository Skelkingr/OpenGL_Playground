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

	bool Update(float deltaTime);
	bool Render();
private:
	void CreateTriangle();
	bool AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
	bool CompileShaders();
private:
	GLuint mVAO;
	GLuint mVBO;
	GLuint mShader;
};

#endif
