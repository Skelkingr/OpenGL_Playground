#include "App.h"

App::App()
	:
	mClientWidth(1360),
	mClientHeight(920),
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
		glm::vec3(0.0f, 1.0f, 5.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		-90.0f,
		0.0f,
		5.0f,
		200.0f
	);

	mMainLight = DirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, 0.0f, glm::vec3(2.0f, 0.0f, -2.0f));

	mPointLights.push_back(PointLight(glm::vec3(0.0f, 0.0f, 1.0f), 0.3f, 0.8f, glm::vec3(4.0f, 1.5f, 0.0f), 0.2f, 0.1f, 0.05f));
	mPointLights.push_back(PointLight(glm::vec3(0.0f, 1.0f, 0.0f), 0.3f, 0.8f, glm::vec3(-4.0f, 1.5f, 0.0f), 0.2f, 0.1f, 0.05f));

	mShinyMaterial = Material(0.8f, 128.0f);
	mDullMaterial = Material(0.3f, 4.0f);
}

App::~App()
{
	for (Texture* texture : mTextureList)
	{
		texture->ClearTexture();
		delete texture;
	}

	for (Mesh* obj : mMeshList)
	{
		obj->ClearMesh();
		delete obj;
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

		Update(0.5f);
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

	CreateObject(true, 0.05f, 1.5f, 0.0005f);
	CreateShader();

	InitTextures();

	for (Mesh* obj : mMeshList)
		obj->SetProjection(glm::perspective(45.0f, (GLfloat)mBufferWidth / (GLfloat)mBufferHeight, 0.1f, 100.0f));

	return true;
}

void App::Update(float deltaTime)
{}

void App::Render()
{
	mShaderList[0].UseShader();
	mShaderList[0].SetDirectionalLight(&mMainLight);
	mShaderList[0].SetPointLights(mPointLights, mPointLights.size());

	// Cube operations:
	glUniformMatrix4fv(mShaderList[0].GetProjectionLocation(), 1, GL_FALSE, glm::value_ptr(mMeshList[0]->GetProjection()));
	glUniformMatrix4fv(mShaderList[0].GetViewLocation(), 1, GL_FALSE, glm::value_ptr(mCamera.CalculateViewMatrix()));
	glUniform3f(
		mShaderList[0].GetEyePositionLocation(),
		mCamera.GetCameraPosition().x,
		mCamera.GetCameraPosition().y,
		mCamera.GetCameraPosition().z
	);

	mMeshList[0]->SetModel(glm::mat4(1.0f));

	mMeshList[0]->SetModel(glm::translate(mMeshList[0]->GetModel(), glm::vec3(0.0f, 1.0f, 0.0f)));
	mMeshList[0]->SetModel(glm::rotate(mMeshList[0]->GetModel(), 45.0f * TO_RADIANS, glm::vec3(0.0f, 1.0f, 0.0f)));

	glUniformMatrix4fv(mShaderList[0].GetModelLocation(), 1, GL_FALSE, glm::value_ptr(mMeshList[0]->GetModel()));

	mTextureList[0]->UseTexture();
	mDullMaterial.UseMaterial(mShaderList[0].GetSpecularIntensityLocation(), mShaderList[0].GetShininessLocation());
	mMeshList[0]->RenderMesh();

	// Floor operations:
	glUniformMatrix4fv(mShaderList[0].GetProjectionLocation(), 1, GL_FALSE, glm::value_ptr(mMeshList[1]->GetProjection()));
	glUniformMatrix4fv(mShaderList[0].GetViewLocation(), 1, GL_FALSE, glm::value_ptr(mCamera.CalculateViewMatrix()));
	glUniform3f(
		mShaderList[0].GetEyePositionLocation(),
		mCamera.GetCameraPosition().x,
		mCamera.GetCameraPosition().y,
		mCamera.GetCameraPosition().z
	);

	mMeshList[1]->SetModel(glm::mat4(1.0f));

	glUniformMatrix4fv(mShaderList[0].GetModelLocation(), 1, GL_FALSE, glm::value_ptr(mMeshList[1]->GetModel()));

	mTextureList[1]->UseTexture();
	mShinyMaterial.UseMaterial(mShaderList[0].GetSpecularIntensityLocation(), mShaderList[0].GetShininessLocation());
	mMeshList[1]->RenderMesh();

	glUseProgram(0);

	glfwSwapBuffers(mMainWindow);
}

void App::CreateObject(bool direction, float offset, float maxOffset, float increment)
{
	std::vector<GLuint> cubeIndices = {};
	std::vector<GLuint> floorIndices = {};

	for (int i = 0; i < 72; i++)
		cubeIndices.push_back(i);

	// X, Y, Z		U, V	NX, NY, NZ
	std::vector<GLfloat> cubeVertices =
	{
		//Front:
		-1.0f, -1.0f,  1.0f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f, // First triangle
		 1.0f, -1.0f,  1.0f,	1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		-1.0f,  1.0f,  1.0f,	0.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-1.0f, -1.0f,  1.0f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f,	1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		-1.0f,  1.0f,  1.0f,	0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		 1.0f, -1.0f,  1.0f,	1.0f, 0.0f,		0.0f, 0.0f, 0.0f, // Second triangle
		 1.0f,  1.0f,  1.0f,	1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-1.0f,  1.0f,  1.0f,	0.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f,	1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f,  1.0f,	1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-1.0f,  1.0f,  1.0f,	0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		//Left:
		-1.0f, -1.0f, -1.0f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f, // First triangle
		-1.0f, -1.0f,  1.0f,	1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		-1.0f,  1.0f, -1.0f,	0.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-1.0f, -1.0f, -1.0f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		-1.0f, -1.0f,  1.0f,	1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		-1.0f,  1.0f, -1.0f,	0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		-1.0f, -1.0f,  1.0f,	1.0f, 0.0f,		0.0f, 0.0f, 0.0f, // Second triangle
		-1.0f,  1.0f,  1.0f,	1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-1.0f,  1.0f, -1.0f,	0.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-1.0f, -1.0f,  1.0f,	1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		-1.0f,  1.0f,  1.0f,	1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-1.0f,  1.0f, -1.0f,	0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		//Back:
		 1.0f, -1.0f, -1.0f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f, // First triangle
		-1.0f, -1.0f, -1.0f,	1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, -1.0f,	0.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, -1.0f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		-1.0f, -1.0f, -1.0f,	1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, -1.0f,	0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		-1.0f, -1.0f, -1.0f,	1.0f, 0.0f,		0.0f, 0.0f, 0.0f, // Second triangle
		-1.0f,	1.0f, -1.0f,	1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, -1.0f,	0.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-1.0f, -1.0f, -1.0f,	1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		-1.0f,	1.0f, -1.0f,	1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, -1.0f,	0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


		//Right:
		 1.0f, -1.0f,  1.0f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f, // First triangle
		 1.0f, -1.0f, -1.0f,	1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f,  1.0f,	0.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, -1.0f,	1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f,  1.0f,	0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		 1.0f, -1.0f, -1.0f,	1.0f, 0.0f,		0.0f, 0.0f, 0.0f, // Second triangle
		 1.0f,  1.0f, -1.0f,	1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f,  1.0f,	0.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, -1.0f,	1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, -1.0f,	1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f,  1.0f,	0.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		
		//Bottom:
		-1.0f, -1.0f,  1.0f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f, // First triangle
		 1.0f, -1.0f,  1.0f,	1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		-1.0f, -1.0f, -1.0f,	0.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-1.0f, -1.0f,  1.0f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f,	1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		-1.0f, -1.0f, -1.0f,	0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		 1.0f, -1.0f,  1.0f,	1.0f, 0.0f,		0.0f, 0.0f, 0.0f, // Second triangle
		 1.0f, -1.0f, -1.0f,	1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-1.0f, -1.0f, -1.0f,	0.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f,	1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, -1.0f,	1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-1.0f, -1.0f, -1.0f,	0.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		
		//Top:
		-1.0f,  1.0f,  1.0f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f, // First triangle
		 1.0f,  1.0f,  1.0f,	1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		-1.0f,  1.0f, -1.0f,	0.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-1.0f,  1.0f,  1.0f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f,  1.0f,	1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		-1.0f,  1.0f, -1.0f,	0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		 1.0f,  1.0f,  1.0f,	1.0f, 0.0f,		0.0f, 0.0f, 0.0f, // Second triangle
		 1.0f,  1.0f, -1.0f,	1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-1.0f,  1.0f, -1.0f,	0.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f,  1.0f,	1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, -1.0f,	1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-1.0f,  1.0f, -1.0f,	0.0f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	floorIndices = {
		0, 2, 1,
		1, 2, 3
	};

	std::vector<GLfloat> floorVertices = {
		-10.0f, 0.0f, -10.0f,	 0.0f,  0.0f,	0.0f, -1.0f, 0.0f,
		 10.0f, 0.0f, -10.0f,	50.0f,  0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f,  10.0f,	 0.0f, 50.0f,	0.0f, -1.0f, 0.0f,
		 10.0f, 0.0f,  10.0f,	50.0f, 50.0f,	0.0f, -1.0f, 0.0f
	};

	ComputeAverageNormals(cubeIndices, cubeIndices.size(), cubeVertices, cubeVertices.size() / 8, 8, 5);

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(cubeVertices, cubeIndices, cubeVertices.size(), cubeIndices.size());
	mMeshList.push_back(obj1);
	
	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(floorVertices, floorIndices, floorVertices.size(), floorIndices.size());
	mMeshList.push_back(obj2);
}

void App::CreateShader()
{
	Shader* shader = new Shader();
	GL_CHECK(shader->CreateFromFiles("Shaders\\shader.vert", "Shaders\\shader.frag"));
	mShaderList.push_back(*shader);
}

void App::InitTextures()
{
	mTextureList.push_back(new Texture((char*)"Textures\\wood.png"));
	mTextureList.push_back(new Texture((char*)"Textures\\plain.png"));
	

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
