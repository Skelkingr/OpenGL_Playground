#ifndef SHADER_H
#define SHADER_H

#include "DirectionalLight.h"
#include "PointLight.h"

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
	GLuint GetEyePositionLocation() const { return mUniformEyePosition; }

	GLuint GetSpecularIntensityLocation() const { return mUniformSpecularIntensity; }
	GLuint GetShininessLocation() const { return mUniformShininess; }

	void SetDirectionalLight(DirectionalLight* directionalLight);

	void UseShader();
	void ClearShader();	

private:
	bool AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
	bool CompileShader(const char* vertexCode, const char* fragmentCode);

private:
	int mPointLightCount;

	struct
	{
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformDirection;
	} mUniformDirectionalLight;

	GLuint mUniformPointLightCount;

	struct
	{
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformPosition;
		GLuint uniformExponent;
		GLuint uniformLinear;
		GLuint uniformConstant;
	} mUniformPointLight[MAX_POINT_LIGHTS];

	GLuint mShaderID;
	GLuint mUniformProjection;
	GLuint mUniformView;
	GLuint mUniformModel;
	GLuint mUniformEyePosition;

	GLuint mUniformSpecularIntensity;
	GLuint mUniformShininess;
};

#endif

