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

	void CreateCube();

private:
	std::vector<Mesh*> mCubeList;

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
