#include "CubeApp.h"

CubeApp::CubeApp()
	:
	App(),
	mVAO(0),
	mVBO1(0),
	mVBO2(0),
	mIBO(0),
	mShader(0),
	mModel(NULL),
	mUniformModel(0),
	mDirection(true),
	mTriOffset(0.0f),
	mTriMaxOffset(0.7f),
	mTriIncrement(0.0005f),
	mCurrAngle(0.0f),
	mSizeDirection(true),
	mCurrSize(0.4f),
	mMaxSize(0.8f),
	mMinSize(0.1f)
{}

CubeApp::~CubeApp()
{}

int CubeApp::Run()
{
	while (!glfwWindowShouldClose(mMainWindow))
	{
		glfwPollEvents();

		Update(0.2f);
		Clear(0.4f, 0.6f, 0.9f, 1.0f);
		Render();
	}

	return 0;
}

bool CubeApp::Init()
{
	if (!App::Init())
		return false;

	CreateCube();

	if (!CompileShaders())
		return false;

	return true;
}

void CubeApp::Update(float deltaTime)
{
	if (mDirection)
	{
		mTriOffset += mTriIncrement * deltaTime;
	}
	else
	{
		mTriOffset -= mTriIncrement * deltaTime;
	}
	if (abs(mTriOffset) >= mTriMaxOffset)
		mDirection = !mDirection;

	mCurrAngle += 0.005f * deltaTime;
	if (mCurrAngle >= 360.0f)
		mCurrAngle -= 360.0f;

	if (mSizeDirection)
	{
		mCurrSize += 0.0001f * deltaTime;
	}
	else
	{
		mCurrSize -= 0.0001f * deltaTime;
	}
	if (mCurrSize <= mMinSize || mCurrSize >= mMaxSize)
		mSizeDirection = !mSizeDirection;
}

void CubeApp::Render()
{
	glUseProgram(mShader);

	mModel = glm::mat4(1.0f);
	mModel = glm::rotate(mModel, mCurrAngle, glm::vec3(0.0f, 1.0f, 1.0f));
	mModel = glm::scale(mModel, glm::vec3(0.4f, 0.4f, 0.4f));
	

	glUniformMatrix4fv(mUniformModel, 1, GL_FALSE, glm::value_ptr(mModel));

	glBindVertexArray(mVAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glUseProgram(0);

	glfwSwapBuffers(mMainWindow);
}

void CubeApp::CreateCube()
{
	const std::vector<GLuint> indices
	{
		//Top
		2, 6, 7,
		2, 3, 7,

		//Bottom
		0, 4, 5,
		0, 1, 5,

		//Left
		0, 2, 6,
		0, 4, 6,

		//Right
		1, 3, 7,
		1, 5, 7,

		//Front
		0, 2, 3,
		0, 1, 3,

		//Back
		4, 6, 7,
		4, 5, 7
	};

	const std::vector<GLfloat> vertices
	{
		 -1, -1,  1.0,
		  1, -1,  1.0,
		 -1,  1,  1.0,
		  1,  1,  1.0,
		 -1, -1, -1.0,
		  1, -1, -1.0,
		 -1,  1, -1.0,
		  1,  1, -1.0
	};

	const std::vector<GLfloat> vertexColors
	{
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f
	};

	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);

	// Index buffer
	glGenBuffers(1, &mIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

	// Vertex buffer
	glGenBuffers(1, &mVBO1);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO1);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	// Vertex color buffer
	glGenBuffers(1, &mVBO2);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO2);
	glBufferData(GL_ARRAY_BUFFER, vertexColors.size() * sizeof(GLfloat), vertexColors.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);

	// Unbind
	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

bool CubeApp::AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
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

bool CubeApp::CompileShaders()
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

	mUniformModel = glGetUniformLocation(mShader, "model");

	return true;
}
