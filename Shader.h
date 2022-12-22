#ifndef SHADER_H
#define SHADER_H

#include "util.h"

class Shader
{
public:
	Shader();
	~Shader();

	void CreateFromString(const char* vertexCode, const char* fragmentCode);

	GLuint GetModelLocation() const { return mUniformModel; }
	GLuint GetProjectionLocation() const { return mUniformProjection; }

	void UseShader();
	void ClearShader();

private:
	bool AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
	bool CompileShader(const char* vertexCode, const char* fragmentCode);

private:
	GLuint mShaderID;
	GLuint mUniformModel;
	GLuint mUniformProjection;
};

#endif

