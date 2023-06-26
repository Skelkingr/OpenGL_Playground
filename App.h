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

	GLvoid Clear(GLfloat r, GLfloat g, GLfloat b, GLfloat a);

	GLboolean Init();
	GLint Run();

	GLvoid RenderScene();
	GLvoid RenderPass(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);

	GLvoid CreateObjects(GLboolean direction, GLfloat offset, GLfloat maxOffset, GLfloat increment);
	GLvoid CreateShaders();

	GLvoid InitCamera();
	
	GLvoid InitDirectionalLight();
	GLvoid InitPointLights();
	GLvoid InitSpotLights();
	GLvoid InitLights();
	
	GLvoid InitMaterials();
	GLvoid InitTextures();

	GLvoid InitModels();

	GLvoid DirectionalShadowMapPass(DirectionalLight* light);
	GLvoid OmniShadowMapPass(PointLight* light);
	
	GLboolean* GetKeys() { return mKeys; }

	GLfloat GetMouseChangeX();
	GLfloat GetMouseChangeY();

private:
	GLboolean InitMainWindow();

	GLvoid CreateCallbacks();
	static GLvoid HandleKeys(GLFWwindow* window, GLint key, GLint code, GLint action, GLint mode);
	static GLvoid HandleMouse(GLFWwindow* window, GLdouble xPos, GLdouble yPos);

private:
	GLint mClientWidth;
	GLint mClientHeight;
	const char* mWindowName;

	GLFWwindow* mMainWindow;

	GLint mBufferWidth;
	GLint mBufferHeight;

	GLuint mUniformProjection;
	GLuint mUniformModel;
	GLuint mUniformView;
	GLuint mUniformEyePosition;
	GLuint mUniformSpecularIntensity;
	GLuint mUniformShininess;
	GLuint mUniformDirectionalLightTransform;
	GLuint mUniformOmniLightPos;
	GLuint mUniformFarPlane;

	std::vector<Mesh*> mMeshList;
	
	std::vector<Shader> mShaderList;
	Shader mDirectionalShadowShader;
	Shader mOmniShadowShader;

	GLboolean mKeys[1024];

	LastMousePos mLastMousePosition;
	MouseChange mMouseChange;

	GLboolean mMouseFirstMoved;

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

