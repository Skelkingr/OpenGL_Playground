#include "App.h"

App::App()
	:
	mClientWidth(800),
	mClientHeight(600),
	mWindowName("Skelkingr"),
	mMainWindow(nullptr)
{}

App::~App()
{}

void App::Clear(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT);
}

int App::Run()
{
	while (!glfwWindowShouldClose(mMainWindow))
	{
		glfwPollEvents();

		Clear(0.0f, 0.0f, 0.0f, 1.0f);
		
		glfwSwapBuffers(mMainWindow);
	}

	return 0;
}

bool App::Init()
{
	if (!InitMainWindow())
	{
		printf("[ERR] Failed to initialize the main window.");
		return false;
	}

	return true;
}

bool App::InitMainWindow()
{
	if (!glfwInit())
	{
		printf("[ERR] GLFW initialisation failed.");
		glfwTerminate();
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Core profile = no backwards compatibility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Allow forward compatibility

	mMainWindow = glfwCreateWindow(mClientWidth, mClientHeight, mWindowName, nullptr, nullptr);
	if (!mMainWindow)
	{
		printf("[ERR] GLFW window creation failed.");
		glfwTerminate();
		return false;
	}

	int bufferWidth;
	int bufferHeight;
	glfwGetFramebufferSize(mMainWindow, &bufferWidth, &bufferHeight);

	// Bind context to window
	glfwMakeContextCurrent(mMainWindow);

	glewExperimental = GL_TRUE; // Allow modern extension features

	if (glewInit() != GLEW_OK)
	{
		printf("[ERR] Glew initialisation failed.");
		glfwDestroyWindow(mMainWindow);
		glfwTerminate();
		return false;
	}

	// Setup viewport size
	glViewport(0, 0, bufferWidth, bufferHeight);

	return true;
}
