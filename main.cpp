#include <stdio.h>
#include <string.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

const GLint CLIENT_WIDTH = 800;
const GLint CLIENT_HEIGHT = 600;

GLuint VAO;
GLuint VBO;
GLuint shader;

static const char* VSHADER = "									\n\
#version 330													\n\
																\n\
layout (location = 0) in vec3 pos;								\n\
																\n\
void main()														\n\
{																\n\
	gl_Position = vec4(0.4 * pos.x, 0.4 * pos.y, pos.z, 1.0);	\n\
}																\n\
";

static const char* FSHADER = "									\n\
#version 330													\n\
																\n\
out vec4 color;													\n\
																\n\
void main()														\n\
{																\n\
	color = vec4(0.0, 1.0, 0.5, 1.0);						 	\n\
}																\n\
";


void CreateTriangle();

void AddShader(GLuint, const char*, GLenum);
void CompileShaders();

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

	CreateTriangle();
	CompileShaders();

	// Loop until window closes
	while (!glfwWindowShouldClose(mainWindow))
	{
		glfwPollEvents();

		// Clear window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		glUseProgram(0);

		glfwSwapBuffers(mainWindow);
	}

	return 0;
}

void CreateTriangle()
{
	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
	GLuint theShader = glCreateShader(shaderType);

	const GLchar* theCode[1] = { 0 };
	theCode[0] = shaderCode;

	GLint codeLength[1] = { 0 };
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(theShader, sizeof(eLog), nullptr, eLog);
		printf("[ERR] Error compiling the %d shader: '%s' \n", shaderType, eLog);
		return;
	}

	glAttachShader(theProgram, theShader);
}

void CompileShaders()
{
	shader = glCreateProgram();

	if (!shader)
	{
		printf("[ERR] Error creating shader program.");
		return;
	}

	AddShader(shader, VSHADER, GL_VERTEX_SHADER);
	AddShader(shader, FSHADER, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), nullptr, eLog);
		printf("[ERR] Error linking program: '%s' \n", eLog);
		return;
	}

	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), nullptr, eLog);
		printf("[ERR] Error validating program: '%s' \n", eLog);
		return;
	}
}