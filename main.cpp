#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

const GLint CLIENT_WIDTH = 800;
const GLint CLIENT_HEIGHT = 600;

int main()
{
	if (!glfwInit())
	{
		printf("[ERR] GLFW initialisation failed.");
		glfwTerminate();
		return 1;
	}
		
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Core profile = no backwards compatibility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Allow forward compatibility

	GLFWwindow* mainWindow = glfwCreateWindow(CLIENT_WIDTH, CLIENT_HEIGHT, "Skelkingr", nullptr, nullptr);
	if (!mainWindow)
	{
		printf("[ERR] GLFW window creation failed.");
		glfwTerminate();
		return 1;
	}

	int bufferWidth;
	int bufferHeight;
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// Bind context to window
	glfwMakeContextCurrent(mainWindow);

	glewExperimental = GL_TRUE; // Allow modern extension features

	if (glewInit() != GLEW_OK)
	{
		printf("[ERR] Glew initialisation failed.");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	// Setup viewport size
	glViewport(0, 0, bufferWidth, bufferHeight);

	// Loop until window closes
	while (!glfwWindowShouldClose(mainWindow))
	{
		glfwPollEvents();

		// Clear window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(mainWindow);
	}

	return 0;
}