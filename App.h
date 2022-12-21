#ifndef APP_H
#define APP_H

#include <stdio.h>
#include <string.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class App
{
public:
	App();
	virtual ~App();

	void Clear(float r, float g, float b, float a);

	virtual int Run();

	virtual bool Init();

	virtual bool Update(float deltaTime) = 0;
	virtual bool Render() = 0;
protected:
	bool InitMainWindow();
protected:
	GLint mClientWidth;
	GLint mClientHeight;
	const char* mWindowName;

	GLFWwindow* mMainWindow;
};

#endif

