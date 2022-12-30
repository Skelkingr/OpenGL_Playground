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

	mDirectionalLight = DirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f), 0.2f, 0.7f, glm::vec3(-2.0f, 0.0f, 2.0f));

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
{
	for (Mesh* obj : mMeshList)
	{
		if (obj->GetDirection())
		{
			obj->SetOffset(obj->GetOffset() + obj->GetIncrement() * deltaTime);
		}
		else
		{
			obj->SetOffset(obj->GetOffset() - obj->GetIncrement() * deltaTime);
		}
		if (abs(obj->GetOffset()) >= obj->GetMaxOffset())
			obj->SetDirection(!obj->GetDirection());

		obj->SetCurrentAngle(obj->GetCurrentAngle() + 0.01f);
		if (obj->GetCurrentAngle() >= 360.0f)
			obj->SetCurrentAngle(obj->GetCurrentAngle() - 360.0f);
	}
}

void App::Render()
{
	int i = 0;

	for (Mesh* obj : mMeshList)
	{
		mShaderList[0].UseShader();
		mShaderList[0].SetDirectionalLight(&mDirectionalLight);

		// Material:
		glUniform1f(mShaderList[0].GetSpecularIntensityLocation(), mShinyMaterial.GetSpecularIntensity());

		// Matrix operations:
		glUniformMatrix4fv(mShaderList[0].GetProjectionLocation(), 1, GL_FALSE, glm::value_ptr(obj->GetProjection()));
		glUniformMatrix4fv(mShaderList[0].GetViewLocation(), 1, GL_FALSE, glm::value_ptr(mCamera.CalculateViewMatrix()));
		glUniform3f(
			mShaderList[0].GetEyePositionLocation(),
			mCamera.GetCameraPosition().x,
			mCamera.GetCameraPosition().y,
			mCamera.GetCameraPosition().z
		);

		// Mesh operations:
		obj->SetModel(glm::mat4(1.0f));

		obj->SetModel(glm::translate(obj->GetModel(), glm::vec3(0.0f, 0.0f, -5.0f)));
		obj->SetModel(glm::rotate(obj->GetModel(), obj->GetCurrentAngle() * TO_RADIANS, glm::vec3(0.0f, 1.0f, 0.0f)));
		//obj->SetModel(glm::scale(obj->GetModel(), glm::vec3(0.3f, 0.3f, 0.3f)));

		glUniformMatrix4fv(mShaderList[0].GetModelLocation(), 1, GL_FALSE, glm::value_ptr(obj->GetModel()));

		mTextureList[i]->UseTexture();
		mShinyMaterial.UseMaterial(mShaderList[0].GetSpecularIntensityLocation(), mShaderList[0].GetShininessLocation());
		obj->RenderMesh();

		glUseProgram(0);

		i++;
	}

	glfwSwapBuffers(mMainWindow);
}

void App::CreateObject(bool direction, float offset, float maxOffset, float increment)
{
	std::vector<GLuint> indices = {};
	for (int i = 0; i < 72; i++)
		indices.push_back(i);

	// X, Y, Z		U, V	NX, NY, NZ
	std::vector<GLfloat> vertices =
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

	ComputeAverageNormals(indices, indices.size(), vertices, vertices.size() / 8, 8, 5);

	Mesh* obj = new Mesh();
	obj->CreateMesh(vertices, indices, vertices.size(), indices.size());
	mMeshList.push_back(obj);
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
	mTextureList.push_back(new Texture((char*)"Textures\\brick.png"));
	mTextureList.push_back(new Texture((char*)"Textures\\dirt.png"));
	

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
