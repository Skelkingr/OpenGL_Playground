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
	mMouseFirstMoved(true),
	mTextureList({})
{
	for (size_t i = 0; i < 1024; i++)
		mKeys[i] = false;

	mCamera = Camera(
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		-90.0f,
		0.0f,
		5.0f,
		200.0f
	);

	mMainLight = Light();
}

App::~App()
{
	for (Texture* texture : mTextureList)
	{
		texture->ClearTexture();
		delete texture;
	}

	for (Mesh* cube : mCubeList)
	{
		cube->ClearMesh();
		delete cube;
	}

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
		mCamera.MouseControl(GetMouseChangeX(), GetMouseChangeY(), deltaTime);

		Update(0.35f);
		Clear(0.0f, 0.0f, 0.0f, 1.0f);
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

	InitTextures();

	for (Mesh* cube : mCubeList)
		cube->SetProjection(glm::perspective(45.0f, (GLfloat)mBufferWidth / (GLfloat)mBufferHeight, 0.1f, 100.0f));

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
	int i = 0;

	for (Mesh* cube : mCubeList)
	{
		mShaderList[0].UseShader();

		mMainLight.UseLight(
			mShaderList[0].GetAmbientIntensityLocation(),
			mShaderList[0].GetAmbientColourLocation()
		);

		cube->SetModel(glm::mat4(1.0f));

		cube->SetModel(glm::translate(cube->GetModel(), glm::vec3(cube->GetOffset(), (GLfloat)(i - 1), -3.0f)));
		cube->SetModel(glm::rotate(cube->GetModel(), cube->GetCurrentAngle() * TO_RADIANS, glm::vec3(0.5f, 0.5f, -0.3f)));
		cube->SetModel(glm::scale(cube->GetModel(), glm::vec3(0.3f, 0.3f, 0.3f)));

		glUniformMatrix4fv(mShaderList[0].GetModelLocation(), 1, GL_FALSE, glm::value_ptr(cube->GetModel()));
		glUniformMatrix4fv(mShaderList[0].GetProjectionLocation(), 1, GL_FALSE, glm::value_ptr(cube->GetProjection()));
		glUniformMatrix4fv(mShaderList[0].GetViewLocation(), 1, GL_FALSE, glm::value_ptr(mCamera.CalculateViewMatrix()));


		mTextureList[i]->UseTexture();
		cube->RenderMesh();

		glUseProgram(0);

		i++;
	}

	glfwSwapBuffers(mMainWindow);
}

void App::CreateObject(bool direction, float offset, float maxOffset, float increment)
{
	std::vector<GLuint> indices =
	{
		//Top:
		0, 2, 3,
		0, 1, 3,

		//Bottom:
		4, 6, 7,
		4, 5, 7,

		//Left:	
		8, 10, 11,
		8, 9, 11,

		//Right:
		12, 14, 15,
		12, 13, 15,
		
		//Front:
		16, 18, 19,
		16, 17, 19,

		//Back:
		20, 22, 23,
		20, 21, 23
	};

	// X, Y, Z		U, V
	std::vector<GLfloat> vertices =
	{
		//Top:
		 -1.0f,  1.0f,  1.0f,	0.0f, 0.0f,
		  1.0f,  1.0f,  1.0f,	1.0f, 0.0f,
		 -1.0f,  1.0f, -1.0f,	0.0f, 1.0f,
		  1.0f,  1.0f, -1.0f,	1.0f, 1.0f,

		//Bottom:
		 -1.0f, -1.0f,  1.0f,	0.0f, 0.0f,
		  1.0f, -1.0f,  1.0f,	1.0f, 0.0f,
		 -1.0f, -1.0f, -1.0f,	0.0f, 1.0f,
		  1.0f, -1.0f, -1.0f,	1.0f, 1.0f,

		//Left:
		 -1.0f, -1.0f, -1.0f,	0.0f, 0.0f,
		 -1.0f, -1.0f,  1.0f,	1.0f, 0.0f,
		 -1.0f,  1.0f, -1.0f,	0.0f, 1.0f,
		 -1.0f,  1.0f,  1.0f,	1.0f, 1.0f,

		//Right:
		  1.0f, -1.0f,  1.0f,	0.0f, 0.0f,
		  1.0f, -1.0f, -1.0f,	1.0f, 0.0f,
		  1.0f,  1.0f,  1.0f,	0.0f, 1.0f,
		  1.0f,  1.0f, -1.0f,	1.0f, 1.0f,

		// Front :
		 -1.0f, -1.0f,  1.0f,	0.0f, 0.0f,
		  1.0f, -1.0f,  1.0f,	1.0f, 0.0f,
		 -1.0f,  1.0f,  1.0f,	0.0f, 1.0f,
		  1.0f,  1.0f,  1.0f,	1.0f, 1.0f,

		//Back:
		  1.0f, -1.0f, -1.0f,	0.0f, 0.0f,
		 -1.0f, -1.0f, -1.0f,	1.0f, 0.0f,
		  1.0f,  1.0f, -1.0f,	0.0f, 1.0f,
		 -1.0f,  1.0f, -1.0f,	1.0f, 1.0f
	};

	Mesh* cube = new Mesh(direction, offset, maxOffset, increment);
	cube->CreateMesh(vertices, indices, vertices.size(), indices.size());
	mCubeList.push_back(cube);
}

void App::CreateShader()
{
	Shader* shader = new Shader();
	shader->CreateFromFiles("Shaders\\shader.vert", "Shaders\\shader.frag");
	mShaderList.push_back(*shader);
}

void App::InitTextures()
{
	mTextureList.push_back(new Texture((char*)"Textures\\brick.png"));
	mTextureList.push_back(new Texture((char*)"Textures\\dirt.png"));
	mTextureList.push_back(new Texture((char*)"Textures\\wood.png"));

	for (Texture* tex : mTextureList)
		tex->LoadTexture();
}

GLfloat App::GetMouseChangeX()
{
	GLfloat theChange = mMouseChange.x;

	mMouseChange.x = 0.0f;

	return theChange;
}

GLfloat App::GetMouseChangeY()
{
	GLfloat theChange = mMouseChange.y;

	mMouseChange.y = 0.0f;

	return theChange;
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
