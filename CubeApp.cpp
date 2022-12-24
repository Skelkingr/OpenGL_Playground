#include "CubeApp.h"

CubeApp::CubeApp()
	:
	App(),
	mCubeList({}),
	mShaderList({}),
	mCamera(nullptr)
{}

CubeApp::~CubeApp()
{
	delete mCamera;

	for (Mesh* cube : mCubeList)
		cube->ClearMesh();
}

int CubeApp::Run()
{
	while (!glfwWindowShouldClose(mMainWindow))
	{
		glfwPollEvents();

		GLfloat deltaTime = mTimer.DeltaTime();

		mCamera->KeyControl(mKeys, deltaTime);
		mCamera->MouseControl(GetMouseChange().x, GetMouseChange().y, deltaTime);

		Update(0.35f);
		Clear(0.4f, 0.6f, 0.9f, 1.0f);
		Render();
	}

	return 0;
}

bool CubeApp::Init()
{
	if (!App::Init())
		return false;

	CreateObject(true, 0.5f, 1.0f, 0.0005f);
	CreateObject(false, 0.0f, 1.0f, 0.0005f);
	CreateObject(true, -0.5f, 1.0f, 0.0005f);

	CreateShader();

	mCamera = new Camera(
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		-90.0f,
		0.0f,
		5.0f,
		100.0f
	);

	for (Mesh* cube : mCubeList)
		cube->SetProjection(glm::perspective(45.0f, (GLfloat)mBufferWidth / (GLfloat)mBufferHeight, 0.1f, 100.0f));
		
	return true;
}

void CubeApp::Update(float deltaTime)
{
	for (Mesh* cube : mCubeList)
	{
		if (cube->GetDirection())
		{
			cube->SetOffset(cube->GetOffset() + cube->GetIncrement() * deltaTime);
		}
		else
		{
			cube->SetOffset(cube->GetOffset() - cube->GetIncrement() * deltaTime);
		}
		if (abs(cube->GetOffset()) >= cube->GetMaxOffset())
			cube->SetDirection(!cube->GetDirection());

		cube->SetCurrentAngle(cube->GetCurrentAngle() + 0.05f);
		if (cube->GetCurrentAngle() >= 360.0f)
			cube->SetCurrentAngle(cube->GetCurrentAngle() - 360.0f);
	}
}

void CubeApp::Render()
{
	float f = -1.0f;
	for (Mesh* cube : mCubeList)
	{
		mShaderList[0].UseShader();

		cube->SetModel(glm::mat4(1.0f));

		cube->SetModel(glm::translate(cube->GetModel(), glm::vec3(cube->GetOffset(), 0.95f * f, -2.5f)));
		cube->SetModel(glm::rotate(cube->GetModel(), cube->GetCurrentAngle() * TO_RADIANS, glm::vec3(0.0f, 1.0f, -1.0f)));
		cube->SetModel(glm::scale(cube->GetModel(), glm::vec3(0.2f, 0.2f, 0.2f)));

		glUniformMatrix4fv(mShaderList[0].GetModelLocation(), 1, GL_FALSE, glm::value_ptr(cube->GetModel()));
		glUniformMatrix4fv(mShaderList[0].GetProjectionLocation(), 1, GL_FALSE, glm::value_ptr(cube->GetProjection()));
		glUniformMatrix4fv(mShaderList[0].GetViewLocation(), 1, GL_FALSE, glm::value_ptr(mCamera->CalculateViewMatrix()));

		cube->RenderMesh();

		glUseProgram(0);

		f += 1.0f;
	}

	glfwSwapBuffers(mMainWindow);
}

void CubeApp::CreateObject(bool direction, float offset, float maxOffset, float increment)
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

	Mesh* cube = new Mesh(direction, offset, maxOffset, increment);
	cube->CreateMesh(vertices, indices, vertexColors, 24, 36, 24);
	mCubeList.push_back(cube);
}

void CubeApp::CreateShader()
{
	Shader* shader = new Shader();
	shader->CreateFromFiles("Shaders\\shader.vert", "Shaders\\shader.frag");
	mShaderList.push_back(*shader);
}
