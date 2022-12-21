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
protected:
	bool InitMainWindow();
protected:
	GLint mClientWidth;
	GLint mClientHeight;
	const char* mWindowName;

	GLFWwindow* mMainWindow;
};

#endif

