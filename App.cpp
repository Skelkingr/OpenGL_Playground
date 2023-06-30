#include "App.h"

App::App()
	:
	mClientWidth(1366),
	mClientHeight(768),
	mWindowName("Skelkingr"),
	mMainWindow(nullptr),
	mBufferWidth(0),
	mBufferHeight(0),
	mUniformProjection(0),
	mUniformModel(0),
	mUniformView(0),
	mUniformEyePosition(0),
	mUniformSpecularIntensity(0),
	mUniformShininess(0),
	mUniformDirectionalLightTransform(0),
	mUniformOmniLightPos(0),
	mUniformFarPlane(0),
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

GLvoid App::Clear(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

GLboolean App::Init()
{
	if (!InitMainWindow())
	{
		std::cout << "[ERR] Failed to initialize the main window." << std::endl;
		return false;
	}

	InitCamera();
	InitLights();
	InitMaterials();
	InitModels();

	CreateObjects(true, 0.05f, 1.5f, 0.0005f);
	CreateShaders();

	InitTextures();

	for (Mesh* obj : mMeshList)
		obj->SetProjection(glm::perspective(glm::radians(60.f), (GLfloat)mBufferWidth / (GLfloat)mBufferHeight, 0.1f, 100.0f));

	return true;
}

GLint App::Run()
{
	glm::mat4 projection = glm::perspective(glm::radians(60.0f), (GLfloat)mBufferWidth / (GLfloat)mBufferHeight, 0.1f, 100.0f);

	while (!glfwWindowShouldClose(mMainWindow))
	{
		glfwPollEvents();

		GLfloat deltaTime = mTimer.DeltaTime();

		mCamera.KeyControl(mKeys, deltaTime);
		mCamera.MouseControl(GetMouseChangeX(), GetMouseChangeY(), deltaTime);

		DirectionalShadowMapPass(&mMainLight);

		
		for (size_t i = 0; i < mPointLights.size(); i++)
		{
			OmniShadowMapPass(&mPointLights[i]);
		}

		for (size_t i = 0; i < mSpotLights.size(); i++)
		{
			OmniShadowMapPass(&mSpotLights[i]);
		}

		RenderPass(mCamera.CalculateViewMatrix(), projection);

		glUseProgram(0);

		glfwSwapBuffers(mMainWindow);
	}

	return 0;
}

GLvoid App::RenderScene()
{
	// Slenderman:
	glm::mat4 model(1.0f);
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 1.95f, -7.0f));
	model = glm::scale(model, glm::vec3(0.006f, 0.006f, 0.006f));
	glUniformMatrix4fv(mUniformModel, 1, GL_FALSE, glm::value_ptr(model));
	mDullMaterial.UseMaterial(mUniformSpecularIntensity, mUniformShininess);
	mSlenderman.RenderModel();

	// Floor:
	model = glm::mat4(1.0f);
	glUniformMatrix4fv(mUniformModel, 1, GL_FALSE, glm::value_ptr(model));
	mTextureList[1]->UseTexture();
	mShinyMaterial.UseMaterial(mUniformSpecularIntensity, mUniformShininess);
	mMeshList[0]->RenderMesh();

	// Wall 1:
	model = glm::mat4(1.0f);
	glUniformMatrix4fv(mUniformModel, 1, GL_FALSE, glm::value_ptr(model));
	mTextureList[2]->UseTexture();
	mDullMaterial.UseMaterial(mUniformSpecularIntensity, mUniformShininess);
	mMeshList[1]->RenderMesh();

	// Wall 2:
	//model = glm::mat4(1.0f);
	//model = glm::rotate(model, -90.0f * TO_RADIANS, glm::vec3(0.0f, 1.0f, 0.0f));
	//glUniformMatrix4fv(mUniformModel, 1, GL_FALSE, glm::value_ptr(model));
	//mTextureList[2]->UseTexture();
	//mDullMaterial.UseMaterial(mUniformSpecularIntensity, mUniformShininess);
	//mMeshList[1]->RenderMesh();

	//// Wall 3:
	//model = glm::mat4(1.0f);
	//model = glm::rotate(model, 180.0f * TO_RADIANS, glm::vec3(0.0f, 1.0f, 0.0f));
	//glUniformMatrix4fv(mUniformModel, 1, GL_FALSE, glm::value_ptr(model));
	//mTextureList[2]->UseTexture();
	//mDullMaterial.UseMaterial(mUniformSpecularIntensity, mUniformShininess);
	//mMeshList[1]->RenderMesh();

	//// Wall 4:
	//model = glm::mat4(1.0f);
	//model = glm::rotate(model, 90.0f * TO_RADIANS, glm::vec3(0.0f, 1.0f, 0.0f));
	//glUniformMatrix4fv(mUniformModel, 1, GL_FALSE, glm::value_ptr(model));
	//mTextureList[2]->UseTexture();
	//mDullMaterial.UseMaterial(mUniformSpecularIntensity, mUniformShininess);
	//mMeshList[1]->RenderMesh();
}

GLvoid App::RenderPass(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{
	mShaderList[0].UseShader();

	mUniformModel = mShaderList[0].GetModelLocation();
	mUniformProjection = mShaderList[0].GetProjectionLocation();
	mUniformView = mShaderList[0].GetViewLocation();
	mUniformModel = mShaderList[0].GetModelLocation();
	mUniformEyePosition = mShaderList[0].GetEyePositionLocation();
	mUniformSpecularIntensity = mShaderList[0].GetSpecularIntensityLocation();
	mUniformShininess = mShaderList[0].GetShininessLocation();

	glViewport(0, 0, mClientWidth, mClientHeight);

	Clear(0.0f, 0.0f, 0.0f, 1.0f);

	glUniformMatrix4fv(mUniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(mUniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniform3f(mUniformEyePosition, mCamera.GetCameraPosition().x, mCamera.GetCameraPosition().y, mCamera.GetCameraPosition().z);

	mShaderList[0].SetDirectionalLight(&mMainLight);
	mShaderList[0].SetPointLights(mPointLights, mPointLights.size(), 3, 0);
	mShaderList[0].SetSpotLights(mSpotLights, mSpotLights.size(), 3 + mSpotLights.size(), mPointLights.size());

	glm::mat4 lightTransform = mMainLight.CalculateLightTransform();
	mShaderList[0].SetDirectionalLightTransform(&lightTransform);

	mMainLight.GetShadowMap()->Read(GL_TEXTURE2);
	mShaderList[0].SetTexture(1);
	mShaderList[0].SetDirectionalShadowMap(2);

	glm::vec3 lowerLight = mCamera.GetCameraPosition();
	lowerLight.y -= 0.3f;
	mSpotLights[0].SetFlash(lowerLight, mCamera.GetCameraDirection());

	mShaderList[0].Validate();

	RenderScene();
}

GLvoid App::CreateObjects(GLboolean direction, GLfloat offset, GLfloat maxOffset, GLfloat increment)
{	

	Mesh* floor = new Mesh();
	floor->CreateMeshFromFile("meshes\\plain\\vertices.txt", "meshes\\plain\\indices.txt", false);
	mMeshList.push_back(floor);

	Mesh* wall = new Mesh();
	wall->CreateMeshFromFile("meshes\\wall\\vertices.txt", "meshes\\wall\\indices.txt", false);
	mMeshList.push_back(wall);
}

GLvoid App::CreateShaders()
{
	Shader* shader = new Shader();
	shader->CreateFromFiles("shaders\\main_shader.vert", "shaders\\main_shader.frag");
	mShaderList.push_back(*shader);
	
	mDirectionalShadowShader = Shader();
	mDirectionalShadowShader.CreateFromFiles("shaders\\directional_shadow_map.vert", "shaders\\directional_shadow_map.frag");
	
	mOmniShadowShader = Shader();
	mOmniShadowShader.CreateFromFiles("shaders\\omni_shadow_map.vert", "shaders\\omni_shadow_map.geom", "shaders\\omni_shadow_map.frag");
}

GLvoid App::InitCamera()
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

GLvoid App::InitDirectionalLight()
{
	mMainLight = DirectionalLight(2048, 2048, glm::vec3(1.0f, 1.0f, 1.0f), 0.05f, 0.1f, glm::vec3(0.0f, -1.0f, -7.0f));
}

GLvoid App::InitPointLights()
{
	mPointLights.push_back(PointLight(1024, 1024, 0.01f, 100.0f, glm::vec3(0.0f, 0.0f, 1.0f), 0.0f, 1.0f, glm::vec3(0.0f, 2.5f, 5.0f), 0.2f, 0.1f, 0.05f));
	mPointLights.push_back(PointLight(1024, 1024, 0.01f, 100.0f, glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 1.0f, glm::vec3(0.0f, 2.5f, -5.0f), 0.2f, 0.1f, 0.05f));
}

GLvoid App::InitSpotLights()
{
	mSpotLights.push_back(
		SpotLight(
			1024,
			1024,
			0.01f,
			100.0f,
			glm::vec3(1.0f, 1.0f, 1.0f),
			1.0f,
			6.0f,
			mCamera.GetCameraPosition(),
			mCamera.GetCameraDirection(),
			0.2f,
			0.1f,
			0.05f,
			20.0f
		)
	);
}

GLvoid App::InitLights()
{
	InitDirectionalLight();
	/*InitPointLights();*/
	InitSpotLights();
}

GLvoid App::InitMaterials()
{
	mShinyMaterial = Material(0.8f, 64.0f);
	mDullMaterial = Material(0.3f, 4.0f);
}

GLvoid App::InitTextures()
{
	mTextureList.push_back(new Texture((char*)"textures\\wood.png"));
	mTextureList.push_back(new Texture((char*)"textures\\plain.png"));
	mTextureList.push_back(new Texture((char*)"textures\\brick.png"));

	for (Texture* tex : mTextureList)
		tex->LoadTextureA();
}

GLvoid App::InitModels()
{
	mSlenderman = Model();
	mSlenderman.LoadModel("models\\slenderman\\slenderman.obj");
}

GLvoid App::DirectionalShadowMapPass(DirectionalLight* light)
{
	mDirectionalShadowShader.UseShader();

	glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight());

	light->GetShadowMap()->Write();
	glClear(GL_DEPTH_BUFFER_BIT);

	mUniformModel = mDirectionalShadowShader.GetModelLocation();

	glm::mat4 lightTransform = mMainLight.CalculateLightTransform();
	mDirectionalShadowShader.SetDirectionalLightTransform(&lightTransform);

	mDirectionalShadowShader.Validate();

	RenderScene();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLvoid App::OmniShadowMapPass(PointLight* light)
{
	mOmniShadowShader.UseShader();

	glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight());

	light->GetShadowMap()->Write();
	glClear(GL_DEPTH_BUFFER_BIT);
	
	mUniformModel = mOmniShadowShader.GetModelLocation();
	mUniformOmniLightPos = mOmniShadowShader.GetOmniLightPosLocation();
	mUniformFarPlane = mOmniShadowShader.GetFarPlaneLocation();

	glUniform3f(mUniformOmniLightPos, light->GetPosition().x, light->GetPosition().y, light->GetPosition().z);
	glUniform1f(mUniformFarPlane, light->GetFarPlane());
	mOmniShadowShader.SetOmniLightMatrices(light->CalculateLightTransform());

	mOmniShadowShader.Validate();

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

GLboolean App::InitMainWindow()
{
	if (!glfwInit())
	{
		std::cout << "[ERR] GLFW initialisation failed." << std::endl;
		glfwTerminate();
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	mMainWindow = glfwCreateWindow(mClientWidth, mClientHeight, mWindowName, nullptr, nullptr);
	if (!mMainWindow)
	{
		std::cout << "[ERR] GLFW window creation failed." << std::endl;
		glfwTerminate();
		return false;
	}

	glfwGetFramebufferSize(mMainWindow, &mBufferWidth, &mBufferHeight);

	// Bind context to window
	glfwMakeContextCurrent(mMainWindow);

	// Handle key + mouse input
	CreateCallbacks();
	glfwSetInputMode(mMainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		std::cout << "[ERR] Glew initialisation failed." << std::endl;
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

GLvoid App::CreateCallbacks()
{
	glfwSetKeyCallback(mMainWindow, HandleKeys);
	glfwSetCursorPosCallback(mMainWindow, HandleMouse);
}

GLvoid App::HandleKeys(GLFWwindow* window, GLint key, GLint code, GLint action, GLint mode)
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

GLvoid App::HandleMouse(GLFWwindow* window, GLdouble xPos, GLdouble yPos)
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
