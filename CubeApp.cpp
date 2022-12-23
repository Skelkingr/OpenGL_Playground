#include "CubeApp.h"

CubeApp::CubeApp()
	:
	App(),
	mCubeList({}),
	mDirection(true),
	mOffset(0.0f),
	mMaxOffset(1.0f),
	mIncrement(0.0005f),
	mCurrAngle(0.0f),
	mSizeDirection(true),
	mCurrSize(0.4f),
	mMaxSize(0.8f),
	mMinSize(0.1f)
{
	
}

CubeApp::~CubeApp()
{
	for (Mesh* cube : mCubeList)
		cube->ClearMesh();
}

int CubeApp::Run()
{
	while (!glfwWindowShouldClose(mMainWindow))
	{
		glfwPollEvents();

		Update(0.3f);
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
	CreateCube();

	for (Mesh* cube : mCubeList)
	{
		if (!cube->CompileShaders())
			return false;

		cube->SetProjection(glm::perspective(45.0f, (GLfloat)mBufferWidth / (GLfloat)mBufferHeight, 0.1f, 100.0f));
	}

	return true;
}

void CubeApp::Update(float deltaTime)
{
	if (mDirection)
	{
		mOffset += mIncrement * deltaTime;
	}
	else
	{
		mOffset -= mIncrement * deltaTime;
	}
	if (abs(mOffset) >= mMaxOffset)
		mDirection = !mDirection;

	mCurrAngle += 0.05f * deltaTime;
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
	float yOffset = -0.75f;

	for (Mesh* cube : mCubeList)
	{
		glUseProgram(cube->GetShader());

		cube->SetModel(glm::mat4(1.0f));

		cube->SetModel(glm::translate(cube->GetModel(), glm::vec3(mOffset, yOffset, -2.5f)));
		cube->SetModel(glm::rotate(cube->GetModel(), mCurrAngle * TO_RADIANS, glm::vec3(0.0f, 1.0f, -1.0f)));
		cube->SetModel(glm::scale(cube->GetModel(), glm::vec3(0.3f, 0.3f, 0.3f)));

		glUniformMatrix4fv(cube->GetUniformModel(), 1, GL_FALSE, glm::value_ptr(cube->GetModel()));
		glUniformMatrix4fv(cube->GetUniformProjection(), 1, GL_FALSE, glm::value_ptr(cube->GetProjection()));

		cube->RenderMesh();

		glUseProgram(0);

		yOffset += 1.5f;
	}

	glfwSwapBuffers(mMainWindow);
}

void CubeApp::CreateCube()
{
	GLuint indices[] =
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

	GLfloat vertices[] =
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

	GLfloat vertexColors[] =
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

	Mesh* cube = new Mesh();
	cube->CreateMesh(vertices, indices, vertexColors, 24, 36, 24);
	mCubeList.push_back(cube);
}
