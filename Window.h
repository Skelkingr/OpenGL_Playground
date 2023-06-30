#ifndef WINDOW_H
#define WINDOW_H

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <iostream>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	~Window();

	GLboolean Initialize();

	GLint GetWidth() const { return mWidth; }
	GLint GetHeight() const { return mHeight; }
	GLint GetBufferWidth() const { return mBufferWidth; }
	GLint GetBufferHeight() const { return mBufferHeight; }

	GLboolean GetShouldClose() { return glfwWindowShouldClose(mMainWindow); }

	GLboolean* GetKeys() { return mKeys; }
	GLfloat GetXChange();
	GLfloat GetYChange();

	GLvoid SwapBuffers() { glfwSwapBuffers(mMainWindow); }

private:
	GLFWwindow* mMainWindow;

	GLint mWidth;
	GLint mHeight;

	GLint mBufferWidth;
	GLint mBufferHeight;

	GLboolean mKeys[1024];

	GLfloat mLastX;
	GLfloat mLastY;
	GLfloat mXChange;
	GLfloat mYChange;
	GLboolean mMouseFirstMoved;

private:
	GLvoid CreateCallBacks();
	static GLvoid HandleKeys(GLFWwindow* window, GLint key, GLint code, GLint action, GLint mode);
	static GLvoid HandleMouse(GLFWwindow* window, GLdouble xPos, GLdouble yPos);
};

#endif
