#ifndef APP_H
#define APP_H

#include "Camera.h"
#include "DirectionalLight.h"
#include "GameTimer.h"
#include "Material.h"
#include "Mesh.h"
#include "SpotLight.h"
#include "Shader.h"
#include "Util.h"
#include "Texture.h"

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

	int Run();

	bool Init();

	void Update(float deltaTime);
	void Render();

	void CreateObject(bool direction, float offset, float maxOffset, float increment);
	void CreateShader();

	void InitTextures();
	
	bool* GetKeys() { return mKeys; }

	GLfloat GetMouseChangeX();
	GLfloat GetMouseChangeY();

private:
	bool InitMainWindow();

	void LoadContent();

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

	bool mKeys[1024];

	LastMousePos mLastMousePosition;
	MouseChange mMouseChange;

	bool mMouseFirstMoved;

	Camera mCamera;
	GameTimer mTimer;

	std::vector<Texture*> mTextureList;

	DirectionalLight mMainLight;
	std::vector<PointLight> mPointLights;

	Material mShinyMaterial;
	Material mDullMaterial;
};

#endif

