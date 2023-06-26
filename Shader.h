#ifndef SHADER_H
#define SHADER_H

#include "DirectionalLight.h"
#include "SpotLight.h"

class Shader
{
public:
	Shader();
	~Shader();

	GLvoid CreateFromString(const char* vertexCode, const char* fragmentCode);
	GLvoid CreateFromFiles(const char* vertexShaderLocation, const char* fragmentShaderLocation);
	GLvoid CreateFromFiles(const char* vertexShaderLocation, const char* geometryShaderLocation, const char* fragmentShaderLocation);

	std::string ReadFile(const char* fileLocation);

	GLuint GetProjectionLocation() const { return mUniformProjection; }
	GLuint GetViewLocation() const { return mUniformView; }
	GLuint GetModelLocation() const { return mUniformModel; }
	GLuint GetEyePositionLocation() const { return mUniformEyePosition; }
	GLuint GetSpecularIntensityLocation() const { return mUniformSpecularIntensity; }
	GLuint GetShininessLocation() const { return mUniformShininess; }
	GLuint GetOmniLightPosLocation() const { return mUniformOmniLightPos; }
	GLuint GetFarPlaneLocation() const { return mUniformFarPlane; }
		
	GLvoid SetUniformModel(GLuint uniformModel) { mUniformModel = uniformModel; }

	GLvoid SetDirectionalLight(DirectionalLight* directionalLight);
	GLvoid SetPointLights(std::vector<PointLight> pointLight, GLuint lightCount);
	GLvoid SetSpotLights(std::vector<SpotLight> pointLight, GLuint lightCount);
	GLvoid SetTexture(GLuint textureUnit);
	GLvoid SetDirectionalShadowMap(GLuint textureUnit);
	GLvoid SetDirectionalLightTransform(glm::mat4* lTransform);
	GLvoid SetLightMatrices(std::vector<glm::mat4> lightMatrices);

	GLvoid UseShader();
	GLvoid ClearShader();	

private:
	GLvoid AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
	GLvoid CompileShader(const char* vertexCode, const char* fragmentCode);
	GLvoid CompileShader(const char* vertexCode, const char* geometryCode, const char* fragmentCode);

	GLvoid CompileProgram();

private:
	GLint mPointLightCount;
	GLint mSpotLightCount;

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

	GLuint mUniformTexture;
	GLuint mUniformDirectionalLightTransform;
	GLuint mUniformDirectionalShadowMap;
	GLuint mUniformOmniLightPos;
	GLuint mUniformFarPlane;

	GLuint mUniformLightMatrices[6];
};

#endif

