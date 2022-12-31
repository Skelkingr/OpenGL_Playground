#ifndef SHADER_H
#define SHADER_H

#include "DirectionalLight.h"
#include "SpotLight.h"

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
	void SetPointLights(std::vector<PointLight> pointLight, unsigned int lightCount);
	void SetSpotLights(std::vector<SpotLight> pointLight, unsigned int lightCount);

	void UseShader();
	void ClearShader();	

private:
	bool AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
	bool CompileShader(const char* vertexCode, const char* fragmentCode);

private:
	int mPointLightCount;
	int mSpotLightCount;

	struct
	{
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformDirection;
	} mUniformDirectionalLight;

	GLuint mUniformPointLightCount;
	GLuint mUniformSpotLightCount;

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

	struct
	{
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformPosition;
		GLuint uniformExponent;
		GLuint uniformLinear;
		GLuint uniformConstant;

		GLuint uniformDirection;
		GLuint uniformEdge;
	} mUniformSpotLight[MAX_SPOT_LIGHTS];

	GLuint mShaderID;
	GLuint mUniformProjection;
	GLuint mUniformView;
	GLuint mUniformModel;
	GLuint mUniformEyePosition;

	GLuint mUniformSpecularIntensity;
	GLuint mUniformShininess;
};

#endif

