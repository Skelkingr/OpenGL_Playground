#ifndef TRIANGLEAPP_H
#define TRIANGLEAPP_H

#include "App.h"
#include "Mesh.h"
#include "Shader.h"

class CubeApp : public App
{
public:
	CubeApp();
	~CubeApp();

	int Run();

	bool Init();

	void Update(float deltaTime);
	void Render();

	void CreateObject(bool direction, float offset, float maxOffset, float increment);
	void CreateShader();

private:
	std::vector<Mesh*> mCubeList;
	std::vector<Shader> mShaderList;
};

#endif
