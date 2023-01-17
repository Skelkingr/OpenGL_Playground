#include "App.h"

App::App()
	:
	mClientWidth(1366),
	mClientHeight(768),
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
}

App::~App()
{
	for (Texture* texture : mTextureList)
	{
		texture->ClearTexture();
		delete texture;
		texture = nullptr;
	}

	for (Mesh* obj : mMeshList)
	{
		obj->ClearMesh();
		delete obj;
		obj = nullptr;
	}

	glfwDestroyWindow(mMainWindow);
	glfwTerminate();
}

void App::Clear(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool App::Init()
{
	if (!InitMainWindow())
	{
		printf("[ERR] Failed to initialize the main window.");
		return false;
	}

	InitCamera();
	InitLights();
	InitMaterials();
	InitModels();

	CreateObjects(true, 0.05f, 1.5f, 0.0005f);
	CreateShader();

	InitTextures();

	for (Mesh* obj : mMeshList)
		obj->SetProjection(glm::perspective(45.0f, (GLfloat)mBufferWidth / (GLfloat)mBufferHeight, 0.1f, 100.0f));

	return true;
}

int App::Run()
{
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)mBufferWidth / (GLfloat)mBufferHeight, 0.1f, 100.0f);

	while (!glfwWindowShouldClose(mMainWindow))
	{
		glfwPollEvents();

		GLfloat deltaTime = mTimer.DeltaTime();

		mCamera.KeyControl(mKeys, deltaTime);
		mCamera.MouseControl(GetMouseChangeX(), GetMouseChangeY(), deltaTime);

		DirectionalShadowMapPass(&mMainLight);
		RenderPass(mCamera.CalculateViewMatrix(), projection);

		glfwSwapBuffers(mMainWindow);
	}

	return 0;
}

void App::RenderScene()
{
	mShaderList[0].UseShader();
	GLuint uniformModel = mShaderList[0].GetModelLocation();
	GLuint uniformProjection = mShaderList[0].GetProjectionLocation();
	GLuint uniformView = mShaderList[0].GetViewLocation();
	GLuint uniformEyePosition = mShaderList[0].GetEyePositionLocation();
	GLuint uniformSpecularIntensity = mShaderList[0].GetSpecularIntensityLocation();
	GLuint uniformShininess = mShaderList[0].GetShininessLocation();

	// Slenderman:
	glm::mat4 model(1.0f);
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 1.95f, -5.0f));
	model = glm::scale(model, glm::vec3(0.006f, 0.006f, 0.006f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	mDullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	mSlenderman.RenderModel();

	// Floor:
	model = glm::mat4(1.0f);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	mTextureList[1]->UseTexture();
	mShinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	mMeshList[0]->RenderMesh();

	// Wall 1:
	model = glm::mat4(1.0f);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	mTextureList[2]->UseTexture();
	mDullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	mMeshList[1]->RenderMesh();

	// Wall 2:
	model = glm::mat4(1.0f);
	model = glm::rotate(model, -90.0f * TO_RADIANS, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	mTextureList[2]->UseTexture();
	mDullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	mMeshList[1]->RenderMesh();

	// Wall 3:
	model = glm::mat4(1.0f);
	model = glm::rotate(model, 180.0f * TO_RADIANS, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	mTextureList[2]->UseTexture();
	mDullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	mMeshList[1]->RenderMesh();

	// Wall 4:
	model = glm::mat4(1.0f);
	model = glm::rotate(model, 90.0f * TO_RADIANS, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	mTextureList[2]->UseTexture();
	mDullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	mMeshList[1]->RenderMesh();
}

void App::RenderPass(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{
	mShaderList[0].UseShader();

	GLuint uniformModel = mShaderList[0].GetModelLocation();
	GLuint uniformProjection = mShaderList[0].GetProjectionLocation();
	GLuint uniformView = mShaderList[0].GetViewLocation();
	GLuint uniformEyePosition = mShaderList[0].GetEyePositionLocation();
	GLuint uniformSpecularIntensity = mShaderList[0].GetSpecularIntensityLocation();
	GLuint uniformShininess = mShaderList[0].GetShininessLocation();

	glViewport(0, 0, mClientWidth, mClientHeight);

	Clear(0.0f, 0.0f, 0.0f, 1.0f);

	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniform3f(uniformEyePosition,mCamera.GetCameraPosition().x, mCamera.GetCameraPosition().y, mCamera.GetCameraPosition().z);

	mShaderList[0].SetDirectionalLight(&mMainLight);
	mShaderList[0].SetPointLights(mPointLights, mPointLights.size());
	mShaderList[0].SetSpotLights(mSpotLights, mSpotLights.size());

	glm::mat4 lightTransform = mMainLight.CalculateLightTransform();
	mShaderList[0].SetDirectionalLightTransform(&lightTransform);

	mMainLight.GetShadowMap()->Read(GL_TEXTURE1);
	mShaderList[0].SetTexture(0);
	mShaderList[0].SetDirectionalShadowMap(1);

	glm::vec3 lowerLight = mCamera.GetCameraPosition();
	lowerLight.y -= 0.3f;
	//mSpotLights[0].SetFlash(lowerLight, mCamera.GetCameraDirection());

	RenderScene();
}

void App::CreateObjects(bool direction, float offset, float maxOffset, float increment)
{	
	Mesh* floor = new Mesh();
	floor->CreateMeshFromFile("Meshes\\Plain\\vertices.txt", "Meshes\\Plain\\indices.txt", false);
	mMeshList.push_back(floor);

	Mesh* wall = new Mesh();
	wall->CreateMeshFromFile("Meshes\\Wall\\vertices.txt", "Meshes\\Wall\\indices.txt", false);
	mMeshList.push_back(wall);
}

void App::CreateShader()
{
	Shader* shader = new Shader();
	shader->CreateFromFiles("Shaders\\shader.vert", "Shaders\\shader.frag");
	mShaderList.push_back(*shader);

	mDirectionalShadowShader = Shader();
	mDirectionalShadowShader.CreateFromFiles("Shaders\\directional_shadow_map.vert", "Shaders\\directional_shadow_map.frag");
}

void App::InitCamera()
{
	mCamera = Camera(
		glm::vec3(0.0f, 1.5f, 7.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		-90.0f,
		0.0f,
		5.0f,
		200.0f
	);
}

void App::InitDirectionalLight()
{
	mMainLight = DirectionalLight(1024, 1024, glm::vec3(1.0f, 1.0f, 1.0f), 0.3f, 0.6f, glm::vec3(0.0f, 0.0f, 0.0f));
}

void App::InitPointLights()
{
	mPointLights.push_back(PointLight(glm::vec3(0.0f, 0.0f, 1.0f), 0.5f, 1.0f, glm::vec3(0.0f, 2.5f, 5.0f), 0.2f, 0.1f, 0.05f));
	mPointLights.push_back(PointLight(glm::vec3(0.0f, 1.0f, 0.0f), 0.5f, 1.0f, glm::vec3(0.0f, 2.5f, -5.0f), 0.2f, 0.1f, 0.05f));
}

void App::InitSpotLights()
{
	mSpotLights.push_back(
		SpotLight(
			glm::vec3(1.0f, 1.0f, 1.0f),
			1.0f,
			4.0f,
			glm::vec3(0.0f, 2.5f, 5.0f),
			glm::vec3(0.0f, -1.0f, 0.0f),
			0.2f,
			0.1f,
			0.05f,
			20.0f
		)
	);
}

void App::InitLights()
{
	InitDirectionalLight();
	//InitPointLights();
	//InitSpotLights();
}

void App::InitMaterials()
{
	mShinyMaterial = Material(0.8f, 64.0f);
	mDullMaterial = Material(0.3f, 4.0f);
}

void App::InitTextures()
{
	mTextureList.push_back(new Texture((char*)"Textures\\wood.png"));
	mTextureList.push_back(new Texture((char*)"Textures\\plain.png"));
	mTextureList.push_back(new Texture((char*)"Textures\\brick.png"));

	for (Texture* tex : mTextureList)
		tex->LoadTextureA();
}

void App::InitModels()
{
	mSlenderman = Model();
	mSlenderman.LoadModel("Models\\slenderman.obj");
}

void App::DirectionalShadowMapPass(DirectionalLight* light)
{
	mDirectionalShadowShader.UseShader();

	glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight());

	light->GetShadowMap()->Write();
	glClear(GL_DEPTH_BUFFER_BIT);

	mDirectionalShadowShader.SetUniformModel(mDirectionalShadowShader.GetModelLocation());

	glm::mat4 lightTransform = light->CalculateLightTransform();
	mDirectionalShadowShader.SetDirectionalLightTransform(&lightTransform);

	RenderScene();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
