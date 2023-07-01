#include "App.h"

App::App()
	:
	mDeltaTime(0.0f),
	mLastTime(0.0f),
	mWindowName("Skelkingr"),
	mMainWindow(),
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
	mTextureList({})
{}

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
}

GLboolean App::Init()
{
	mMainWindow = Window(1366, 768);

	if (!mMainWindow.Initialize())
	{
		std::cout << "[ERR] Failed to initialize the main window." << std::endl;
		return false;
	}

	CreateObjects();
	CreateShaders();

	InitCamera();
	InitTextures();
	InitMaterials();
	InitModels();
	InitLights(true, false, true);
	InitSkybox();

	for (Mesh* obj : mMeshList)
		obj->SetProjection(glm::perspective(glm::radians(60.f), (GLfloat)mMainWindow.GetBufferWidth() / (GLfloat)mMainWindow.GetBufferHeight(), 0.1f, 100.0f));

	return true;
}

GLint App::Run()
{
	glm::mat4 projection = glm::perspective(glm::radians(60.0f), (GLfloat)mMainWindow.GetBufferWidth() / (GLfloat)mMainWindow.GetBufferHeight(), 0.1f, 100.0f);

	while (!mMainWindow.GetShouldClose())
	{
		GLfloat now = (GLfloat)glfwGetTime();
		mDeltaTime = now - mLastTime;
		mLastTime = now;

		glfwPollEvents();

		mCamera.KeyControl(mMainWindow.GetKeys(), mDeltaTime);
		mCamera.MouseControl(mMainWindow.GetXChange(), mMainWindow.GetYChange(), mDeltaTime);

		if (mMainWindow.GetKeys()[GLFW_KEY_L])
		{
			mSpotLights[0].Toggle();
			mMainWindow.GetKeys()[GLFW_KEY_L] = false;
		}	

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

		mMainWindow.SwapBuffers();
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
	//model = glm::mat4(1.0f);
	//glUniformMatrix4fv(mUniformModel, 1, GL_FALSE, glm::value_ptr(model));
	//mTextureList[2]->UseTexture();
	//mDullMaterial.UseMaterial(mUniformSpecularIntensity, mUniformShininess);
	//mMeshList[1]->RenderMesh();

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
	glViewport(0, 0, mMainWindow.GetWidth(), mMainWindow.GetHeight());

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mSkybox.DrawSkybox(viewMatrix, projectionMatrix);

	mShaderList[0].UseShader();

	mUniformModel = mShaderList[0].GetModelLocation();
	mUniformProjection = mShaderList[0].GetProjectionLocation();
	mUniformView = mShaderList[0].GetViewLocation();
	mUniformModel = mShaderList[0].GetModelLocation();
	mUniformEyePosition = mShaderList[0].GetEyePositionLocation();
	mUniformSpecularIntensity = mShaderList[0].GetSpecularIntensityLocation();
	mUniformShininess = mShaderList[0].GetShininessLocation();

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

GLvoid App::CreateObjects()
{
	Mesh* floor = new Mesh();
	floor->CreateMeshFromFile("meshes\\plain\\vertices.txt", "meshes\\plain\\indices.txt", true);
	mMeshList.push_back(floor);

	Mesh* wall = new Mesh();
	wall->CreateMeshFromFile("meshes\\wall\\vertices.txt", "meshes\\wall\\indices.txt", true);
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
	mPointLights.push_back(PointLight(1024, 1024, 0.01f, 100.0f, glm::vec3(0.0f, 0.0f, 1.0f), 0.0f, 0.4f, glm::vec3(0.0f, 2.5f, 5.0f), 0.3f, 0.01f, 0.01f));
	mPointLights.push_back(PointLight(1024, 1024, 0.01f, 100.0f, glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.4f, glm::vec3(0.0f, 2.5f, -5.0f), 0.3f, 0.01f, 0.01f));
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

GLvoid App::InitLights(GLboolean directional, GLboolean point, GLboolean spot)
{
	if (directional)
		InitDirectionalLight();
	if (point)
		InitPointLights();
	if (spot)
		InitSpotLights();
}

GLvoid App::InitSkybox()
{
	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("textures\\skybox\\nightsky_rt.tga");
	skyboxFaces.push_back("textures\\skybox\\nightsky_lf.tga");
	skyboxFaces.push_back("textures\\skybox\\nightsky_up.tga");
	skyboxFaces.push_back("textures\\skybox\\nightsky_dn.tga");
	skyboxFaces.push_back("textures\\skybox\\nightsky_bk.tga");
	skyboxFaces.push_back("textures\\skybox\\nightsky_ft.tga");

	mSkybox = Skybox(skyboxFaces);
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
