#include "TriangleApp.h"

TriangleApp::TriangleApp()
	:
	App(),
	mVAO(0),
	mVBO(0),
	mShader(0)
{}

TriangleApp::~TriangleApp()
{}

int TriangleApp::Run()
{
	// Loop until window closes
	while (!glfwWindowShouldClose(mMainWindow))
	{
		glfwPollEvents();

		Clear(0.0f, 0.0f, 0.0f, 1.0f);

		glUseProgram(mShader);

		glBindVertexArray(mVAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		glUseProgram(0);

		glfwSwapBuffers(mMainWindow);
	}

	return 0;
}

bool TriangleApp::Init()
{
	if (!App::Init())
		return false;

	CreateTriangle();

	if (!CompileShaders())
		return false;

	return true;
}

bool TriangleApp::Update(float deltaTime)
{
	return false;
}

bool TriangleApp::Render()
{
	return false;
}

void TriangleApp::CreateTriangle()
{
	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 0.0f,  1.0f, 0.0f
	};

	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);

	glGenBuffers(1, &mVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

bool TriangleApp::AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
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

bool TriangleApp::CompileShaders()
{
	mShader = glCreateProgram();

	if (!mShader)
	{
		printf("[ERR] Error creating shader program.");
		return false;
	}

	AddShader(mShader, VSHADER, GL_VERTEX_SHADER);
	AddShader(mShader, FSHADER, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(mShader);
	glGetProgramiv(mShader, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(mShader, sizeof(eLog), nullptr, eLog);
		printf("[ERR] Error linking program: '%s' \n", eLog);
		return false;
	}

	glValidateProgram(mShader);
	glGetProgramiv(mShader, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(mShader, sizeof(eLog), nullptr, eLog);
		printf("[ERR] Error validating program: '%s' \n", eLog);
		return false;
	}

	return true;
}
