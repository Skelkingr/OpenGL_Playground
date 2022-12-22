#ifndef TRIANGLEAPP_H
#define TRIANGLEAPP_H

#include "App.h"
#include "Mesh.h"

class CubeApp : public App
{
public:
	CubeApp();
	~CubeApp();

	int Run();

	bool Init();

	void Update(float deltaTime);
	void Render();

private:
	Mesh* mCube;

	bool mDirection;
	float mOffset;
	float mMaxOffset;
	float mIncrement;

	float mCurrAngle;

	bool mSizeDirection;
	float mCurrSize;
	float mMaxSize;
	float mMinSize;
};

#endif
