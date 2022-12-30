#include "Shader.h"

Shader::Shader()
	:
	mPointLightCount(0),
	mUniformDirectionalLight(),
	mUniformPointLightCount(0),
	mUniformPointLight(),
	mShaderID(0),
	mUniformModel(0),
	mUniformProjection(0),
	mUniformView(0),
	mUniformEyePosition(0),
	mUniformSpecularIntensity(0),
	mUniformShininess(0)
{}

Shader::~Shader()
{
	ClearShader();
}

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

void Shader::SetPointLights(PointLight* pointLight, unsigned int lightCount)
{
	if (lightCount > MAX_POINT_LIGHTS)
		lightCount = MAX_POINT_LIGHTS;

	glUniform1i(mUniformPointLightCount, lightCount);

	for (size_t i = 0; i < lightCount; i++)
		pointLight->UseLight(
			mUniformPointLight[i].uniformColour,
			mUniformPointLight[i].uniformAmbientIntensity,
			mUniformPointLight[i].uniformDiffuseIntensity,
			mUniformPointLight[i].uniformPosition,
			mUniformPointLight[i].uniformExponent,
			mUniformPointLight[i].uniformLinear,
			mUniformPointLight[i].uniformConstant
		);
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

	return true;
}
