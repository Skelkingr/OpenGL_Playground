#ifndef APP_H
#define APP_H

#include "Camera.h"
#include "DirectionalLight.h"
#include "Material.h"
#include "Model.h"
#include "SpotLight.h"
#include "Shader.h"
#include "Util.h"
#include "Window.h"

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

	GLboolean Init();
	GLint Run();

	GLvoid RenderScene();
	GLvoid RenderPass(glm::mat4 projectionMatrix, glm::mat4 viewMatrix);

	GLvoid CreateObjects(GLboolean direction, GLfloat offset, GLfloat maxOffset, GLfloat increment);
	GLvoid CreateShaders();

	GLvoid InitCamera();
	GLvoid InitDirectionalLight();
	GLvoid InitPointLights();
	GLvoid InitSpotLights();
	GLvoid InitMaterials();
	GLvoid InitTextures();
	GLvoid InitModels();

	GLvoid DirectionalShadowMapPass(DirectionalLight* light);
	GLvoid OmniShadowMapPass(PointLight* light);

private:
	GLfloat mDeltaTime;
	GLfloat mLastTime;

	Window mMainWindow;
	const char* mWindowName;

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

	LastMousePos mLastMousePosition;
	MouseChange mMouseChange;

	Camera mCamera;

	std::vector<Texture*> mTextureList;

	DirectionalLight mMainLight;
	std::vector<PointLight> mPointLights;
	std::vector<SpotLight> mSpotLights;

	Material mShinyMaterial;
	Material mDullMaterial;

	Model mSlenderman;
};

#endif

