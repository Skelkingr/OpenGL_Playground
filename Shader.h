#ifndef SHADER_H
#define SHADER_H

#include "Util.h"

class Shader
{
public:
	Shader();
	~Shader();

	void CreateFromString(const char* vertexCode, const char* fragmentCode);
	void CreateFromFiles(const char* vertexShaderLocation, const char* fragmentShaderLocation);

	std::string ReadFile(const char* fileLocation);

	GLuint GetProjectionLocation() const { return mUniformProjection; }
	GLuint GetViewLocation() const { return mUniformView; }
	GLuint GetModelLocation() const { return mUniformModel; }
	GLuint GetAmbientColourLocation() const { return mUniformAmbientColour; }
	GLuint GetAmbientIntensityLocation() const { return mUniformAmbientIntensity; }
	GLuint GetDiffuseColourLocation() const { return mUniformDiffuseColour; }
	GLuint GetDiffuseIntensityLocation() const { return mUniformDiffuseIntensity; }
	GLuint GetDirectionLocation() const { return mUniformDirection; }

	void UseShader();
	void ClearShader();	

private:
	bool AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
	bool CompileShader(const char* vertexCode, const char* fragmentCode);

private:
	GLuint mShaderID;
	GLuint mUniformProjection;
	GLuint mUniformView;
	GLuint mUniformModel;
	GLuint mUniformAmbientColour;
	GLuint mUniformAmbientIntensity;
	GLuint mUniformDiffuseColour;
	GLuint mUniformDirection;
	GLuint mUniformDiffuseIntensity;
};

#endif

