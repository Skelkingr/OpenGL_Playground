#ifndef APP_H
#define APP_H

#include "util.h"

class App
{
public:
	App();
	virtual ~App();

	void Clear(float r, float g, float b, float a);

	virtual int Run();

	virtual bool Init();

	virtual void Update(float deltaTime) = 0;
	virtual void Render() = 0;

	virtual void CreateObject(bool direction, float offset, float maxOffset, float increment) = 0;

protected:
	bool InitMainWindow();

	void CreateCallbacks();
	static void HandleKeys(GLFWwindow* window, int key, int code, int action, int mode);

protected:
	GLint mClientWidth;
	GLint mClientHeight;
	const char* mWindowName;

	GLFWwindow* mMainWindow;

	int mBufferWidth;
	int mBufferHeight;

	bool mKeys[1024];
};

#endif

