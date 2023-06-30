#include "Window.h"

Window::Window()
	:
	mMainWindow(nullptr),
	mBufferWidth(0),
	mBufferHeight(0),
	mLastX(0.0f),
	mLastY(0.0f),
	mMouseFirstMoved(false)
{
	mWidth = 800;
	mHeight = 600;

	for (size_t i = 0; i < 1024; i++)
	{
		mKeys[i] = false;
	}

	mXChange = 0.0f;
	mYChange = 0.0f;
}

Window::Window(GLint windowWidth, GLint windowHeight)
{
	mWidth = windowWidth;
	mHeight = windowHeight;

	for (size_t i = 0; i < 1024; i++)
	{
		mKeys[i] = false;
	}

	mXChange = 0.0f;
	mYChange = 0.0f;
}

Window::~Window()
{
	glfwDestroyWindow(mMainWindow);
	glfwTerminate();
}

GLboolean Window::Initialize()
{
	if (!glfwInit())
	{
		std::cout << "[ERR] GLFW initialisation failed." << std::endl;
		glfwTerminate();
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	mMainWindow = glfwCreateWindow(mWidth, mHeight, "Skelkingr", nullptr, nullptr);
	if (!mMainWindow)
	{
		std::cout << "[ERR] GLFW window creation failed." << std::endl;
		glfwTerminate();
		return false;
	}

	// Get buffer size information
	glfwGetFramebufferSize(mMainWindow, &mBufferWidth, &mBufferHeight);

	// Bind context to window
	glfwMakeContextCurrent(mMainWindow);

	// Handle key + mouse input
	CreateCallBacks();
	glfwSetInputMode(mMainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glewExperimental = GL_TRUE;

	GLenum status = glewInit();
	if ( status != GLEW_OK)
	{
		std::cout << "[ERR] Glew initialisation failed: " << glewGetErrorString(status) << std::endl;
		glfwDestroyWindow(mMainWindow);
		glfwTerminate();
		return false;
	}

	glEnable(GL_DEPTH_TEST);

	// Create Viewport
	glViewport(0, 0, mBufferWidth, mBufferHeight);

	glfwSetWindowUserPointer(mMainWindow, this);

	return true;
}

GLfloat Window::GetXChange()
{
	GLfloat change = mXChange;
	mXChange = 0.0f;
	return change;
}

GLfloat Window::GetYChange()
{
	GLfloat change = mYChange;
	mYChange = 0.0f;
	return change;
}

GLvoid Window::CreateCallBacks()
{
	glfwSetKeyCallback(mMainWindow, HandleKeys);
	glfwSetCursorPosCallback(mMainWindow, HandleMouse);
}

GLvoid Window::HandleKeys(GLFWwindow* window, GLint key, GLint code, GLint action, GLint mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key <= 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->mKeys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->mKeys[key] = false;
		}
	}
}

GLvoid Window::HandleMouse(GLFWwindow* window, GLdouble xPos, GLdouble yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mMouseFirstMoved)
	{
		theWindow->mLastX = (GLfloat)xPos;
		theWindow->mLastY = (GLfloat)yPos;

		theWindow->mMouseFirstMoved = false;
	}

	theWindow->mXChange = (GLfloat)xPos - theWindow->mLastX;
	theWindow->mYChange = theWindow->mLastY - (GLfloat)yPos;

	theWindow->mLastX = (GLfloat)xPos;
	theWindow->mLastY = (GLfloat)yPos;
}


