#include "CubeApp.h"

CubeApp::CubeApp()
	:
	App(),
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
	mCube = new Mesh();
	
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

	mCube->SetIndices(indices);
	mCube->SetVertices(vertices);
	mCube->SetVertexColors(vertexColors);
}

CubeApp::~CubeApp()
{
	delete mCube;
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

	mCube->CreateMesh();

	if (!mCube->CompileShaders())
		return false;

	mCube->SetProjection(glm::perspective(45.0f, (GLfloat)mBufferWidth / (GLfloat)mBufferHeight, 0.1f, 100.0f));

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
	glUseProgram(mCube->GetShader());

	mCube->SetModel(glm::mat4(1.0f));
	
	mCube->SetModel(glm::translate(mCube->GetModel(), glm::vec3(mOffset, 0.0f, -2.5f)));
	mCube->SetModel(glm::rotate(mCube->GetModel(), mCurrAngle * TO_RADIANS, glm::vec3(0.0f, 1.0f, -1.0f)));
	mCube->SetModel(glm::scale(mCube->GetModel(), glm::vec3(0.4f, 0.4f, 0.4f)));
	
	glUniformMatrix4fv(mCube->GetUniformModel(), 1, GL_FALSE, glm::value_ptr(mCube->GetModel()));
	glUniformMatrix4fv(mCube->GetUniformProjection(), 1, GL_FALSE, glm::value_ptr(mCube->GetProjection()));

	glBindVertexArray(mCube->GetVAO());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mCube->GetIBO());

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glUseProgram(0);

	glfwSwapBuffers(mMainWindow);
}
