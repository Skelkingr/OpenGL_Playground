#ifndef APP_H
#define APP_H

#include "Camera.h"
#include "DirectionalLight.h"
#include "GameTimer.h"
#include "Material.h"
#include "Model.h"
#include "SpotLight.h"
#include "Shader.h"
#include "Util.h"

struct LastMousePos
{
	GLfloat x;
	GLfloat y;
};

struct MouseChange
{
	GLfloat x;
	GLfloat y;
};

class App
{
public:
	App();
	~App();

	void Clear(float r, float g, float b, float a);

	bool Init();
	int Run();

	void RenderScene();
	void RenderPass(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);

	void CreateObjects(bool direction, float offset, float maxOffset, float increment);
	void CreateShaders();

	void InitCamera();
	
	void InitDirectionalLight();
	void InitPointLights();
	void InitSpotLights();
	void InitLights();
	
	void InitMaterials();
	void InitTextures();

	void InitModels();

	void DirectionalShadowMapPass(DirectionalLight* light);
	
	bool* GetKeys() { return mKeys; }

	GLfloat GetMouseChangeX();
	GLfloat GetMouseChangeY();

private:
	bool InitMainWindow();

	void CreateCallbacks();
	static void HandleKeys(GLFWwindow* window, int key, int code, int action, int mode);
	static void HandleMouse(GLFWwindow* window, double xPos, double yPos);

private:
	GLint mClientWidth;
	GLint mClientHeight;
	const char* mWindowName;

	GLFWwindow* mMainWindow;

	int mBufferWidth;
	int mBufferHeight;

	std::vector<Mesh*> mMeshList;
	std::vector<Shader> mShaderList;
	Shader mDirectionalShadowShader;

	bool mKeys[1024];

	LastMousePos mLastMousePosition;
	MouseChange mMouseChange;

	bool mMouseFirstMoved;

	Camera mCamera;
	GameTimer mTimer;

	std::vector<Texture*> mTextureList;

	DirectionalLight mMainLight;
	std::vector<PointLight> mPointLights;
	std::vector<SpotLight> mSpotLights;

	Material mShinyMaterial;
	Material mDullMaterial;

	Model mSlenderman;
};

#endif

