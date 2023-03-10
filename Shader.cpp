#include "Shader.h"

Shader::Shader()
	:
	mPointLightCount(0),
	mSpotLightCount(0),
	mUniformDirectionalLight(),
	mUniformPointLightCount(0),
	mUniformSpotLightCount(0),
	mUniformPointLight(),
	mUniformSpotLight(),
	mShaderID(0),
	mUniformModel(0),
	mUniformProjection(0),
	mUniformView(0),
	mUniformEyePosition(0),
	mUniformSpecularIntensity(0),
	mUniformShininess(0),
	mUniformTexture(0),
	mUniformDirectionalLightTransform(0),
	mUniformDirectionalShadowMap(0)
{}

Shader::~Shader()
{}

void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode)
{
	CompileShader(vertexCode, fragmentCode);
}

void Shader::CreateFromFiles(const char* vertexShaderLocation, const char* fragmentShaderLocation)
{
	std::string vertexString = ReadFile(vertexShaderLocation);
	std::string fragmentString = ReadFile(fragmentShaderLocation);

	const char* vertexCode = vertexString.c_str();
	const char* fragmentCode = fragmentString.c_str();

	CompileShader(vertexCode, fragmentCode);
}

std::string Shader::ReadFile(const char* fileLocation)
{
	std::string content;
	std::ifstream fileStream(fileLocation, std::ios::in);

	if (!fileStream.is_open())
	{
		printf("[ERR] Failed to read %s. File doesn't exist.", fileLocation);
		return "";
	}

	std::string line = "";
	while (!fileStream.eof())
	{
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();

	return content;
}

void Shader::SetDirectionalLight(DirectionalLight* directionalLight)
{
	directionalLight->UseLight(
		mUniformDirectionalLight.uniformColour,
		mUniformDirectionalLight.uniformAmbientIntensity,
		mUniformDirectionalLight.uniformDiffuseIntensity,
		mUniformDirectionalLight.uniformDirection
	);
}

void Shader::SetPointLights(std::vector<PointLight> pointLight, unsigned int lightCount)
{
	if (lightCount > MAX_POINT_LIGHTS)
		lightCount = MAX_POINT_LIGHTS;

	glUniform1i(mUniformPointLightCount, lightCount);

	for (size_t i = 0; i < lightCount; i++)
		pointLight[i].UseLight(
			mUniformPointLight[i].uniformColour,
			mUniformPointLight[i].uniformAmbientIntensity,
			mUniformPointLight[i].uniformDiffuseIntensity,
			mUniformPointLight[i].uniformPosition,
			mUniformPointLight[i].uniformExponent,
			mUniformPointLight[i].uniformLinear,
			mUniformPointLight[i].uniformConstant
		);
}


void Shader::SetSpotLights(std::vector<SpotLight> spotLight, unsigned int lightCount)
{
	if (lightCount > MAX_SPOT_LIGHTS)
		lightCount = MAX_SPOT_LIGHTS;

	glUniform1i(mUniformSpotLightCount, lightCount);

	for (size_t i = 0; i < lightCount; i++)
		spotLight[i].UseLight(
			mUniformSpotLight[i].uniformColour,
			mUniformSpotLight[i].uniformAmbientIntensity,
			mUniformSpotLight[i].uniformDiffuseIntensity,
			mUniformSpotLight[i].uniformPosition,
			mUniformSpotLight[i].uniformDirection,
			mUniformSpotLight[i].uniformExponent,
			mUniformSpotLight[i].uniformLinear,
			mUniformSpotLight[i].uniformConstant,
			mUniformSpotLight[i].uniformEdge
		);
}

void Shader::SetTexture(GLuint textureUnit)
{
	glUniform1i(mUniformTexture, textureUnit);
}

void Shader::SetDirectionalShadowMap(GLuint textureUnit)
{
	glUniform1i(mUniformDirectionalShadowMap, textureUnit);
}

void Shader::SetDirectionalLightTransform(glm::mat4* lTransform)
{
	glUniformMatrix4fv(mUniformDirectionalLightTransform, 1, GL_FALSE, glm::value_ptr(*lTransform));
}

void Shader::UseShader()
{
	glUseProgram(mShaderID);
}

void Shader::ClearShader()
{
	if (mShaderID != 0)
	{
		glDeleteProgram(mShaderID);
		mShaderID = 0;
	}

	mUniformModel = 0;
	mUniformProjection = 0;
}

bool Shader::AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
	GLuint theShader = glCreateShader(shaderType);

	const GLchar* theCode[1] = { 0 };
	theCode[0] = shaderCode;

	GLint codeLength[1] = { 0 };
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(theShader, sizeof(eLog), nullptr, eLog);
		printf("[ERR] Error compiling the %d shader: '%s' \n", shaderType, eLog);
		return false;
	}

	glAttachShader(theProgram, theShader);

	return true;
}

bool Shader::CompileShader(const char* vertexCode, const char* fragmentCode)
{
	mShaderID = glCreateProgram();

	if (!mShaderID)
	{
		printf("[ERR] Error creating shader program.");
		return false;
	}

	AddShader(mShaderID, vertexCode, GL_VERTEX_SHADER);
	AddShader(mShaderID, fragmentCode, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(mShaderID);
	glGetProgramiv(mShaderID, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(mShaderID, sizeof(eLog), nullptr, eLog);
		printf("[ERR] Error linking program: '%s' \n", eLog);
		return false;
	}

	glValidateProgram(mShaderID);
	glGetProgramiv(mShaderID, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(mShaderID, sizeof(eLog), nullptr, eLog);
		printf("[ERR] Error validating program: '%s' \n", eLog);
		return false;
	}

	mUniformProjection = glGetUniformLocation(mShaderID, "projection");
	mUniformView = glGetUniformLocation(mShaderID, "view");
	mUniformModel = glGetUniformLocation(mShaderID, "model");
	mUniformEyePosition = glGetUniformLocation(mShaderID, "eyePosition");

	mUniformDirectionalLight.uniformColour = glGetUniformLocation(mShaderID, "directionalLight.base.colour");
	mUniformDirectionalLight.uniformAmbientIntensity = glGetUniformLocation(mShaderID, "directionalLight.base.ambientIntensity");
	mUniformDirectionalLight.uniformDiffuseIntensity = glGetUniformLocation(mShaderID, "directionalLight.base.diffuseIntensity");
	mUniformDirectionalLight.uniformDirection = glGetUniformLocation(mShaderID, "directionalLight.direction");

	mUniformSpecularIntensity = glGetUniformLocation(mShaderID, "material.specularIntensity");
	mUniformShininess = glGetUniformLocation(mShaderID, "material.shininess");
	
	mUniformPointLightCount = glGetUniformLocation(mShaderID, "pointLightCount");
	for (size_t i = 0; i < MAX_POINT_LIGHTS; i++)
	{
		char locBuff[100] = { '\0' };

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.colour", i);
		mUniformPointLight[i].uniformColour = glGetUniformLocation(mShaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.ambientIntensity", i);
		mUniformPointLight[i].uniformAmbientIntensity = glGetUniformLocation(mShaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.diffuseIntensity", i);
		mUniformPointLight[i].uniformDiffuseIntensity = glGetUniformLocation(mShaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].position", i);
		mUniformPointLight[i].uniformPosition = glGetUniformLocation(mShaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].exponent", i);
		mUniformPointLight[i].uniformExponent = glGetUniformLocation(mShaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].linear", i);
		mUniformPointLight[i].uniformLinear = glGetUniformLocation(mShaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].constant", i);
		mUniformPointLight[i].uniformConstant = glGetUniformLocation(mShaderID, locBuff);
	}

	mUniformSpotLightCount = glGetUniformLocation(mShaderID, "spotLightCount");
	for (size_t i = 0; i < MAX_SPOT_LIGHTS; i++)
	{
		char locBuff[100] = { '\0' };

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.colour", i);
		mUniformSpotLight[i].uniformColour = glGetUniformLocation(mShaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.ambientIntensity", i);
		mUniformSpotLight[i].uniformAmbientIntensity = glGetUniformLocation(mShaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.diffuseIntensity", i);
		mUniformSpotLight[i].uniformDiffuseIntensity = glGetUniformLocation(mShaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.position", i);
		mUniformSpotLight[i].uniformPosition = glGetUniformLocation(mShaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.exponent", i);
		mUniformSpotLight[i].uniformExponent = glGetUniformLocation(mShaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.linear", i);
		mUniformSpotLight[i].uniformLinear = glGetUniformLocation(mShaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.constant", i);
		mUniformSpotLight[i].uniformConstant = glGetUniformLocation(mShaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].direction", i);
		mUniformSpotLight[i].uniformDirection = glGetUniformLocation(mShaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].edge", i);
		mUniformSpotLight[i].uniformEdge = glGetUniformLocation(mShaderID, locBuff);
	}

	mUniformTexture = glGetUniformLocation(mShaderID, "theTexture");
	mUniformDirectionalLightTransform = glGetUniformLocation(mShaderID, "directionalLightTransform");
	mUniformDirectionalShadowMap = glGetUniformLocation(mShaderID, "directionalShadowMap");

	return true;
}
