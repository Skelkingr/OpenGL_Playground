#include "Shader.h"

Shader::Shader()
	:
	mShaderID(0),
	mUniformModel(0),
	mUniformProjection(0)
{}

Shader::~Shader()
{
	ClearShader();
}

void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode)
{
	CompileShader(vertexCode, fragmentCode);
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

	mUniformModel = glGetUniformLocation(mShaderID, "model");
	mUniformProjection = glGetUniformLocation(mShaderID, "projection");

	return true;
}