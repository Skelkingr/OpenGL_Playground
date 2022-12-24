#include "App.h"

App::App()
	:
	mClientWidth(800),
	mClientHeight(600),
	mWindowName("Skelkingr"),
	mMainWindow(nullptr),
	mBufferWidth(0),
	mBufferHeight(0),
	mLastMousePosition({ 0.0f, 0.0f }),
	mMouseChange({ 0.0f, 0.0f }),
	mMouseFirstMoved(true)
{
	for (size_t i = 0; i < 1024; i++)
		mKeys[i] = false;

	mCamera = Camera(
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		-90.0f,
		0.0f,
		5.0f,
		100.0f
	);
}

App::~App()
{
	for (Mesh* cube : mCubeList)
		cube->ClearMesh();

	glfwDestroyWindow(mMainWindow);
	glfwTerminate();
}

void App::Clear(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

int App::Run()
{
	while (!glfwWindowShouldClose(mMainWindow))
	{
		glfwPollEvents();

		GLfloat deltaTime = mTimer.DeltaTime();

		mCamera.KeyControl(mKeys, deltaTime);
		mCamera.MouseControl(GetMouseChange().x, GetMouseChange().y, deltaTime);

		Update(0.35f);
		Clear(0.4f, 0.6f, 0.9f, 1.0f);
		Render();
	}

	return 0;
}

bool App::Init()
{
	if (!InitMainWindow())
	{
		printf("[ERR] Failed to initialize the main window.");
		return false;
	}

	CreateObject(true, 0.5f, 1.0f, 0.0005f);
	CreateObject(false, 0.0f, 1.0f, 0.0005f);
	CreateObject(true, -0.5f, 1.0f, 0.0005f);

	CreateShader();

	for (Mesh* cube : mCubeList)
		cube->SetProjection(glm::perspective(45.0f, (GLfloat)mBufferWidth / (GLfloat)mBufferHeight, 0.1f, 100.0f));

	return true;

	return true;
}

void App::Update(float deltaTime)
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

void App::Render()
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
		glUniformMatrix4fv(mShaderList[0].GetViewLocation(), 1, GL_FALSE, glm::value_ptr(mCamera.CalculateViewMatrix()));

		cube->RenderMesh();

		glUseProgram(0);

		f += 1.0f;
	}

	glfwSwapBuffers(mMainWindow);
}

void App::CreateObject(bool direction, float offset, float maxOffset, float increment)
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

void App::CreateShader()
{
	Shader* shader = new Shader();
	shader->CreateFromFiles("Shaders\\shader.vert", "Shaders\\shader.frag");
	mShaderList.push_back(*shader);
}

MouseChange App::GetMouseChange()
{
	GLfloat theChangeX = mMouseChange.x;
	GLfloat theChangeY = mMouseChange.y;

	mMouseChange.x = 0.0f;
	mMouseChange.y = 0.0f;

	return MouseChange{ theChangeX, theChangeY };
}

bool App::InitMainWindow()
{
	if (!glfwInit())
	{
		printf("[ERR] GLFW initialisation failed.");
		glfwTerminate();
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Core profile = no backwards compatibility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Allow forward compatibility

	mMainWindow = glfwCreateWindow(mClientWidth, mClientHeight, mWindowName, nullptr, nullptr);
	if (!mMainWindow)
	{
		printf("[ERR] GLFW window creation failed.");
		glfwTerminate();
		return false;
	}

	glfwGetFramebufferSize(mMainWindow, &mBufferWidth, &mBufferHeight);

	// Bind context to window
	glfwMakeContextCurrent(mMainWindow);

	// Handle key + mouse input
	CreateCallbacks();
	glfwSetInputMode(mMainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glewExperimental = GL_TRUE; // Allow modern extension features

	if (glewInit() != GLEW_OK)
	{
		printf("[ERR] Glew initialisation failed.");
		glfwDestroyWindow(mMainWindow);
		glfwTerminate();
		return false;
	}

	// Enable depth
	glEnable(GL_DEPTH_TEST);

	// Setup viewport size
	glViewport(0, 0, mBufferWidth, mBufferHeight);

	glfwSetWindowUserPointer(mMainWindow, this);

	return true;
}

void App::CreateCallbacks()
{
	glfwSetKeyCallback(mMainWindow, HandleKeys);
	glfwSetCursorPosCallback(mMainWindow, HandleMouse);
}

void App::HandleKeys(GLFWwindow* window, int key, int code, int action, int mode)
{

	App* theApp = static_cast<App*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key <= 1024)
	{
		if (action == GLFW_PRESS)
		{
			theApp->mKeys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			theApp->mKeys[key] = false;
		}
	}
}

void App::HandleMouse(GLFWwindow* window, double xPos, double yPos)
{
	App* theApp = static_cast<App*>(glfwGetWindowUserPointer(window));

	if (theApp->mMouseFirstMoved)
	{
		theApp->mLastMousePosition.x = (GLfloat)xPos;
		theApp->mLastMousePosition.y = (GLfloat)yPos;

		theApp->mMouseFirstMoved = false;
	}

	theApp->mMouseChange.x = (GLfloat)xPos - theApp->mLastMousePosition.x;
	theApp->mMouseChange.y = theApp->mLastMousePosition.y - (GLfloat)yPos;

	theApp->mLastMousePosition.x = (GLfloat)xPos;
	theApp->mLastMousePosition.y = (GLfloat)yPos;
}
