#ifndef APP_H
#define APP_H

#include "util.h"

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
	virtual ~App();

	void Clear(float r, float g, float b, float a);

	virtual int Run();

	virtual bool Init();

	virtual void Update(float deltaTime) = 0;
	virtual void Render() = 0;

	virtual void CreateObject(bool direction, float offset, float maxOffset, float increment) = 0;
	
	bool* GetKeys() { return mKeys; }
	MouseChange GetChange();

protected:
	bool InitMainWindow();

	void CreateCallbacks();
	static void HandleKeys(GLFWwindow* window, int key, int code, int action, int mode);
	static void HandleMouse(GLFWwindow* window, double xPos, double yPos);

protected:
	GLint mClientWidth;
	GLint mClientHeight;
	const char* mWindowName;

	GLFWwindow* mMainWindow;

	int mBufferWidth;
	int mBufferHeight;

	bool mKeys[1024];

	LastMousePos mLastMousePosition;
	MouseChange mMouseChange;

	bool mMouseFirstMoved;
};

#endif

